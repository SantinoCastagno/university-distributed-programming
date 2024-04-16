package rmi.client;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import rmi.central.CentralInterface;

/**
 * Client
 */
public class ClientObject {

  public static String entry = "";

  public static void main(String[] args) {
    String[] predictions = new String[2];
    try {

      Registry myRegistry = LocateRegistry.getRegistry("127.0.0.1", 8080);
      CentralInterface centralServer = (CentralInterface) myRegistry.lookup("CentralObject");

      // Scanner scanner = new Scanner(System.in);

      // System.out.print("Enter the sign and date: ");

      // entry = scanner.nextLine();

      predictions = centralServer.requestCentral("horosocpo clima");

      System.out.println("Client LOG: \n -> " + predictions[0] + "\n -> " + predictions[1]);

      // scanner.close();

    } catch (Exception e) {
      e.printStackTrace();
    }
  }

}
