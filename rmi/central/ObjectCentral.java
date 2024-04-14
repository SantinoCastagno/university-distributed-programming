package rmi.central;

import java.rmi.server.UnicastRemoteObject;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class ObjectCentral extends UnicastRemoteObject {

  public ObjectCentral() throws RemoteException {
    super();
  }

  public static void main(String[] args) {
    try {
      Registry myRegistry = LocateRegistry.createRegistry(8080);
      myRegistry.rebind("ObjectCentral", new Request());
      System.out.println("LOG: Central server ON");

    } catch (Exception e) {
      System.err.println("Error in client: " + e.getMessage());
    }
  }

}
