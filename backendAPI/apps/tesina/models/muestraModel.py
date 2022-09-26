from typing import Optional
import uuid
from pydantic import BaseModel, Field


class MuestraModel(BaseModel):
    id: str = Field(default_factory=uuid.uuid4, alias="_id")
    origen: str = Field(...)
    sensor: str = Field(...)
    valor: str = Field(...)
    
    ##Example for FastApi
    class Config:
        allow_population_by_field_name = True
        schema_extra = {
            "example": {
                "id": "00010203-0405-0607-0809-0a0b0c0d0e0f",
                "origen": "deDondeViene",
                "sensor": "queSensorEs",
                "valor": "unValor"
            }
        }


class UpdateMuestraModel(BaseModel):
    origen: Optional[str]
    sensor: Optional[str]
    valor: Optional[str]

    ##Example for FastApi   
    class Config:
        schema_extra = {
            "example": {
                "origen": "deDondeViene",
                "sensor": "queSensorEs",
                "valor": "unValor",
            }
        }