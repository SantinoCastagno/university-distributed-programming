#!/usr/bin/env python

import asyncio
import time
import websockets
from websockets import connect


async def run_connection():
    async for websocket in connect("ws://localhost:8765"):
        try:
            while True:
                await websocket.send("h")
                message = await websocket.recv()
                print(f"Received: {message}")
                time.sleep(5)
        except websockets.ConnectionClosed:
            time.sleep(5)
            print("LOG: connection failure, reconnecting..")
            continue

asyncio.run(run_connection())