#!/usr/bin/env python

import asyncio
import time
import websockets
from websockets import connect

async def hello():
    while True:
        try:
            async with connect("ws://localhost:8765") as ws:
                while True:
                    await ws.send("Hello world!")
                    message = await ws.recv()
                    print(f"Received: {message}")
                    time.sleep(10)
        except websockets.ConnectionClosed:
            continue

async def run_connection():
    async for websocket in connect("ws://localhost:8765"):
        try:
            while True:
                        await websocket.send("Hello world!")
                        message = await websocket.recv()
                        print(f"Received: {message}")
                        time.sleep(5)
        except websockets.ConnectionClosed:
            print("LOG: connection failure, reconnecting..")
            continue

asyncio.run(run_connection())