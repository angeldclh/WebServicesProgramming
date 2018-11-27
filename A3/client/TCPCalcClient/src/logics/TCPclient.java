/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package logics;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

/**
 *
 * @author angel
 */
public class TCPclient {

    private static final String HOSTNAME = "localhost";
    private static final int PORT = 50001; //redirected by VirtualBox
    private final Socket sock;

    public TCPclient() throws IOException {
        this.sock = new Socket(HOSTNAME, PORT);
    }

    public Socket getSock() {
        return this.sock;
    }

    //Sends the calculation to te server and returns the result as a String
    public String calculateInServer(int n1, int n2, char op) throws IOException {
        if (op != '\u0000') { //If no operation, don't send anything to server to avoid segfault
            //Send to server
            PrintWriter out = new PrintWriter(sock.getOutputStream(), true);
            out.println(Double.toString(n1) + Character.toString(op) + Double.toString(n2));

            //Receive from server
            BufferedReader in = new BufferedReader(new InputStreamReader(sock.getInputStream()));
            return in.readLine();
        }
        return "0";
    }
}
