package rmi;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface weatherInterface extends java.rmi.Remote {
  public String consultWeather(String entry) throws RemoteException;

}
