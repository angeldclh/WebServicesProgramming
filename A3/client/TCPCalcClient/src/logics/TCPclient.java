/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package logics;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;

/**
 *
 * @author angel
 */
public class TCPclient {

    private static String hostName = "localhost";
    private static int port = 50001; //redirected by VirtualBox
    private final Socket sock;

    public TCPclient() throws IOException {
        //TODO crear socket TCP
        this.sock = new Socket(hostName, port);
    }

    //Sends the calculation to te server and returns the result as a String
    public String calculateInServer(int n1, int n2, char op) throws IOException {
        //Send to server
        DataOutputStream out = new DataOutputStream(sock.getOutputStream());
        out.writeUTF(Double.toString(n1) + op + Double.toString(n2));
        out.flush();

        //Receive from server
        DataInputStream in = new DataInputStream(sock.getInputStream());

        return in.readUTF();
    }
}
