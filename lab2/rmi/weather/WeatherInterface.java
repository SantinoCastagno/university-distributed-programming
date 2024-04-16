package rmi.weather;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface WeatherInterface extends java.rmi.Remote {
  public String consultWeather(String entry) throws RemoteException;

}
