from datetime import datetime
from fastapi import APIRouter, Body, Request, HTTPException, status
from fastapi.responses import JSONResponse
from fastapi.encoders import jsonable_encoder


from ..models.muestraModel import MuestraModel, UpdateMuestraModel

muestraRouter = APIRouter()


@muestraRouter.post("/", response_description="Add new muestra")
async def create_muestra(request: Request, muestra: MuestraModel = Body(...)):
    muestra = jsonable_encoder(muestra)
    new_muestra = await request.app.mongodb["muestras"].insert_one(muestra)
    created_muestra = await request.app.mongodb["muestras"].find_one(
        {"_id": new_muestra.inserted_id}
    )

    return JSONResponse(status_code=status.HTTP_201_CREATED, content=created_muestra)


@muestraRouter.get("/", response_description="List all muestras")
async def list_muestras(request: Request):
    muestras = []
    for doc in await request.app.mongodb["muestras"].find().to_list(length=100):
        muestras.append(doc)
    return muestras


@muestraRouter.get("/{id}", response_description="Get a single muestra")
async def show_muestra(id: str, request: Request):
    if (muestra := await request.app.mongodb["muestras"].find_one({"_id": id})) is not None:
        return muestra

    raise HTTPException(status_code=404, detail=f"Muestra {id} not found")


@muestraRouter.put("/{id}", response_description="Update a muestra")
async def update_muestra(id: str, request: Request, muestra: UpdateMuestraModel = Body(...)):
    muestra = {k: v for k, v in muestra.dict().items() if v is not None}

    if len(muestra) >= 1:
        update_result = await request.app.mongodb["muestras"].update_one(
            {"_id": id}, {"$set": muestra}
        )

        if update_result.modified_count == 1:
            if (
                updated_muestra := await request.app.mongodb["muestras"].find_one({"_id": id})
            ) is not None:
                return updated_muestra

    if (
        existing_muestra := await request.app.mongodb["muestras"].find_one({"_id": id})
    ) is not None:
        return existing_muestra

    raise HTTPException(status_code=404, detail=f"Muestra {id} not found")


@muestraRouter.delete("/{id}", response_description="Delete Muestra")
async def delete_muestra(id: str, request: Request):
    delete_result = await request.app.mongodb["muestras"].delete_one({"_id": id})

    if delete_result.deleted_count == 1:
        return JSONResponse(status_code=status.HTTP_204_NO_CONTENT)

    raise HTTPException(status_code=404, detail=f"Muestra {id} not found")

#Todo: This is to quick and dirty, please refactorize
@muestraRouter.get("/sensor/{sensor}", response_description="Get last sensor reading")
async def show_sensor(sensor: str, request: Request):
    muestras = []
    for doc in await request.app.mongodb["muestras"].find({"sensor": sensor}).to_list(length=1000):
        muestras.append(doc)
    return muestras.pop()



