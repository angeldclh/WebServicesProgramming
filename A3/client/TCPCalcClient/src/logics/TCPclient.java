/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package logics;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
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
        PrintWriter out = new PrintWriter(sock.getOutputStream(), true);
        out.println(Double.toString(n1) + Character.toString(op) + Double.toString(n2));

        //Receive from server
        BufferedReader in = new BufferedReader(new InputStreamReader(sock.getInputStream()));

        return in.readLine();
    }
}
