while true
do
    ./server_horoscope &
    sleep 1
    ./server_weather &
    sleep 1
    ./server_central &
    sleep 1
    ./client capricornio 14-42-23
    sleep 1
    ./client sagitarius 21-21-21
done