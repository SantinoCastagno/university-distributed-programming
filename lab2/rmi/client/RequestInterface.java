package rmi.client;

import java.rmi.*;

public interface RequestInterface extends Remote {
    public String[] request(String entry) throws RemoteException;
}
