from fastapi import FastAPI
import logging
import uvicorn
from motor.motor_asyncio import AsyncIOMotorClient
from fastapi.middleware.cors import CORSMiddleware
import os
from urllib.parse import quote_plus

from fastapi import FastAPI, Request, status
from fastapi.exceptions import RequestValidationError
from fastapi.responses import JSONResponse

from fastapi.middleware.trustedhost import TrustedHostMiddleware


from app.controllers.muestraController import muestraRouter

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger("FastApi")

#Running on http://localhost:8000/docs#/
app = FastAPI()

origins = ["*"]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

#Incluir los routers
app.include_router(muestraRouter, tags=["muestras"], prefix="/api")



@app.on_event("startup")
async def startup_db_client():
    host = os.getenv('MONGODB_HOST', '')
    username = os.getenv('MONGODB_USER', '')
    password = os.getenv('MONGODB_PASSWORD', '')
    port = int(os.getenv('MONGODB_PORT', 27017))
    dbname = os.getenv('MONGODB_DBNAME', '')
    endpoint = 'mongodb://{0}:{1}@{2}'.format(quote_plus(username),
                                              quote_plus(password), host)
    app.mongodb_client = AsyncIOMotorClient(endpoint, port)
    app.mongodb = app.mongodb_client[dbname]


@app.on_event("shutdown")
async def shutdown_db_client():
    app.mongodb_client.close()

@app.exception_handler(RequestValidationError)
async def validation_exception_handler(request: Request, exc: RequestValidationError):
	exc_str = f'{exc}'.replace('\n', ' ').replace('   ', ' ')
	logging.error(f"{request}: {exc_str}")
	content = {'status_code': 10422, 'message': exc_str, 'data': None}
	return JSONResponse(content=content, status_code=status.HTTP_422_UNPROCESSABLE_ENTITY)

# Custom middleware to log request and response information
@app.middleware("http")
async def custom_logging_middleware(request: Request, call_next):
    request_info = f"  {request.client.host}:{request.client.port} - \"{request.method} {request.url.path}\""
    content = await request.body()     
    response = await call_next(request)   
    logger.info(request_info)
    logger.info(f"  Contenido:")
    logger.info(f"            {content.decode()}")
    logger.info('--------------------------------------------------------------------------------------------------')
    return response