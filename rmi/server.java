package TP2;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface server extends java.rmi.Remote {
    public String consult(String entry) throws RemoteException;

}
