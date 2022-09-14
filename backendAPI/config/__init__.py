from pydantic import BaseSettings


class CommonSettings(BaseSettings):
    APP_NAME: str = "TesinaAPI"
    DEBUG_MODE: bool = False


class ServerSettings(BaseSettings):
    HOST: str = "0.0.0.0"
    PORT: int = 8000


class DatabaseSettings(BaseSettings):
    DB_URL: str = "mongodb://localhost:27017"
    DB_NAME: str = "TesinaDB"



class Settings(CommonSettings, ServerSettings, DatabaseSettings):
    pass


settings = Settings()
