import json

arduinoEnvia = {"tipo": "tanque", "tipoFertilizante": "NPK", "nivel": 75}
arduinoEnvia2 = {"tipo": "manometro", "presion": 2.5}
mensajes = [arduinoEnvia, arduinoEnvia2]

for mensaje_json in mensajes:
    mensaje = json.dumps(mensaje_json)

    datos = json.loads(mensaje)
    print("Datos JSON:", datos)
    print(type(datos))

    if datos["tipo"] == "tanque":
        datos["topico"] = f"tanque/{datos['tipoFertilizante']}"
        print("Clasificado:", datos)
    elif datos["tipo"] == "manometro":
        datos["topico"] = "manometro/presion"
        print("Clasificado:", datos)

