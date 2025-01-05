from pydantic import BaseModel


class Settings(BaseModel):
    database_hostname: str="localhost"
    database_port: str="5432"
    database_password: str="2104"
    database_name: str="fastapi"
    database_username: str="postgres"
    secret_key: str="09d25e094faa6ca7099f6f0f4caa6cf63b88e8d3e7"
    algorithm: str="HS256"
    access_token_expire_minutes: int = 300

    class Config:
        env_file = ".env"

settings = Settings()

