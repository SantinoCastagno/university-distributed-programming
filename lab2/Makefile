# Deletes all .class files
clean:
	@echo "\n---- Deleting all .class files ----"
	@find rmi -name '*.class' -print -delete

# Compiles all the .java
compile: clean
	@echo "\n---- Compiling all .java files ----"
	@find rmi -name '*.java' -print -exec javac {} +

# Executes all the objects in parallel
run: compile
	@echo "\n---- Running all .class files ----"
	@java rmi.horoscope.HoroscopeObject & sleep 1
	@java rmi.weather.WeatherObject & sleep 1
	@java rmi.central.CentralObject & sleep 1
	@java rmi.client.ClientObject

# Deletes, compiles and runs the objects
all: run

# Kills all the java rmi processes
kill:
	pkill -f 'java rmi'