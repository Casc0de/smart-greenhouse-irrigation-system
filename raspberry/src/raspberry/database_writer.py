from pymongo import MongoClient
from pymongo import ServerApi

uri = "mongodb://python_user:5961#Spiderman@127.0.0.1:27017"

client = MongoClient(uri, server_api=ServerApi('1'))

db = client["invernadero"]
collection = db["environment_readings"]

