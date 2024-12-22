from fastapi import FastAPI
from passlib.context import CryptContext
from .database import engine
from .routers import user, post, auth
from . import models

pwd_context = CryptContext(schemes=["bcrypt"], deprecated="auto")

models.Base.metadata.create_all(bind=engine)

app = FastAPI()

app.include_router(post.router)
app.include_router(user.router)
app.include_router(auth.router)



@app.get("/")
async def root():
    return {"message": "Welcome to FastAPI!"}
