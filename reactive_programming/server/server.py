#!/usr/bin/env python
import asyncio
from websockets.server import serve
from reactivex import Subject

msg_list = Subject()

# def on_next(ws, msg_list):
#     print("Hola")
    
def on_error(e):
    print("Error Occurred: {0}".format(e))
    
def on_completed():
    print("Clientes actualizados.")

async def handler(ws):
    msg_list.subscribe(
        on_next  = lambda i: print("Received {0}".format(i)),
        on_error = on_error,
        on_completed = on_completed,
    )
    async for message in ws:
        msg_list.on_next(message)
        

async def main():
    async with serve(handler, "localhost", 8765):
        await asyncio.Future()  # run forever

asyncio.run(main())