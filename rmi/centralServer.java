package TP2;

import java.rmi.server.UnicastRemoteObject;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class centralServer extends UnicastRemoteObject implements server {

  public centralServer() throws RemoteException {
    super();
  }

  @Override
  public String consult(String entry) {
    return "Hola";
  }

  public static void main(String[] args) {
    try {
      Registry myRegistry = LocateRegistry.createRegistry(1234);
      myRegistry.rebind("centralServer", new centralServer());
      System.out.println("Central server ON");

    } catch (Exception e) {
      System.err.println("Error in client: " + e.getMessage());
    }
  }

}
