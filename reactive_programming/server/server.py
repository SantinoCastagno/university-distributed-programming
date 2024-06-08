#!/usr/bin/env python

import asyncio
from websockets.server import serve

ws_list = []

async def handler(ws):
    ws_list.append(ws)
    async for message in ws:
        print(ws_list)
        await ws.send(message)

async def main():
    async with serve(handler, "localhost", 8765):
        await asyncio.Future()  # run forever

asyncio.run(main())