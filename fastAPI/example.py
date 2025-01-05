import os


#print out the enviroment value of the path:  variable: MY_DB_URL, and value of it
print(os.environ.get('MY_DB_URL'))


class Settings(BaseModel):
    database_password: str="localhost"
    database_username: str="postgres"
    secret_key: str ="09d25e094faa6ca7099f6f0f4caa6cf63b88e8d3e7"

settings= Settings()
