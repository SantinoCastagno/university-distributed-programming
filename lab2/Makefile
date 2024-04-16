# Compiles all the .java
all:
	javac rmi/central/*.java
	javac rmi/client/*.java

# Run the central object
central_object:
	java -Djava.security.policy=central.permissions rmi.central.ObjectCentral

# Run the client
client:	
	java -Djava.security.policy=central.permissions rmi.client.Client

clean:
	rm rmi/central/*.class
	rm rmi/client/*.class