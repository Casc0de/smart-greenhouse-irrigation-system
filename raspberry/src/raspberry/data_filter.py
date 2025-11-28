import asyncio

class DataFilter:
    def __init__(self, filter_queue, db_queue, thresholds):
        self.filter_queue = filter_queue
        self.db_queue = db_queue
        self.thresholds = thresholds  # Diccionario con los umbrales para cada tipo de dato
        self.last_values = {}  # Para almacenar los últimos valores recibidos

    def is_relevant(self, dato):
        
        tipo = dato["topico"]
        valor = dato["valor"]
        print(f"[DATA_filter] \n tipo: {tipo} \n valor: {valor}")
        if tipo not in self.last_values:
            self.last_values[tipo] = valor
            return True  # Primer dato, siempre pasa el filtro
        

        if abs(valor - self.last_values[tipo]) >= self.thresholds[tipo]:
            self.last_values[tipo] = valor
            return True  # El dato pasa el filtro
        
        return False  # El dato no pasa el filtro
    
    async def filtrar_datos(self):
        print("[DATA_filter] \n Iniciando el filtro de datos...")
        while True:
            dato = await self.filter_queue.get()
            if self.is_relevant(dato):
                print(f"Dato relevante: {dato}")
                await self.db_queue.put(dato)
            else:
                print(f"Dato descartado por el filtro: {dato}")