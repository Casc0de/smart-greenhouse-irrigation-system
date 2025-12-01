import asyncio
from datetime import datetime
from motor.motor_asyncio import AsyncIOMotorClient

class DatabaseWriter:
    def __init__(self, db_queue, 
                 MONGO_URI, 
                 DB_NAME):
        self.mongo_uri = MONGO_URI
        self.db_name = DB_NAME
        self.db_queue = db_queue

    async def start(self):
        self.client = AsyncIOMotorClient(self.mongo_uri)
        self.db = self.client[self.db_name]
        self.collections = {
            "tank_levels": self.db["tank_levels"],
            "irrigation_pressure": self.db["irrigation_pressure"],
            "environment_readings": self.db["environment_readings"],
            "soil_readings": self.db["soil_readings"]
        }

        while True:
            data = await self.db_queue.get()
            await self.write_to_db(data)
            

    async def write_to_db(self, data):
        
        collection_name = data["collection"]
        if collection_name not in self.collections:
            print(f"[Database_Writer] \n [WARNING] \n Colección no encontrada: {collection_name}")
            return
        
        collection = self.collections[collection_name]

        document = {k: v for k, v in data.items() if k != "collection"}

        result = await collection.insert_one(document)
        print(f"[Database_Writer] Documento insertado con ID: {result.inserted_id}")