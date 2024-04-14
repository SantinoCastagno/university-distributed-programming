package rmi;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface HoroscopeInterface extends java.rmi.Remote {
  public String consultHoroscope(String entry) throws RemoteException;

}
