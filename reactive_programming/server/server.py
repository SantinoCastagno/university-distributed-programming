#!/usr/bin/env python
import asyncio
from websockets.server import serve
import reactivex

def push_five_strings(observer, scheduler):
    observer.on_next("Alpha")
    observer.on_next("Beta")
    observer.on_next("Gamma")
    observer.on_next("Delta")
    observer.on_next("Epsilon")
    observer.on_completed()

observable_chat = reactivex.create(push_five_strings)
    
def on_error(e):
    print("Error Occurred: {0}".format(e))
    
def on_completed():
    print("Clientes actualizados.")

async def handler(ws):
    observable_chat.subscribe()
    async for message in ws:
        print(message)
        

async def main():
    async with serve(handler, "localhost", 8765):
        await asyncio.Future()  # run forever

asyncio.run(main())