package com.cgroup.androidcontrolinterface;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

/**
 * Created by Teng on 2015/9/20.
 */
public class UDPClientClass{
    /** Constructor */
    protected DatagramSocket clientSocket;
    protected byte[] receiveData = new byte[3];
    protected byte[] sendData = new byte[3];

    public UDPClientClass(){
    }

    /** Description: By using the function client, we can sent a message
     *  Parameter: #1 ip address in String, #2 port in int, #3 sentence to sent in String
     *  Return: Non
     * */
    public void send(String ip_address, int port) throws IOException {
        //send_data = sentence.getBytes();
        InetAddress IPAddress =  InetAddress.getByName(ip_address);

        DatagramPacket send_packet = new DatagramPacket(sendData,3, IPAddress, port);
        try {
            clientSocket.send(send_packet);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    /** Description: By using the function receive, we can receive the message from the target
     *  Parameter: #1 ip address in String, #2 port in int
     *  Return: state in Boolean, true for the success and false for the failure
     * */
    public Boolean receive(String ip_address, int port) throws IOException {
        InetAddress IPAddress =  InetAddress.getByName(ip_address);
        DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
        Boolean state;
        try {
            clientSocket.receive(receivePacket);
            state = true;
        } catch (IOException e) {
            e.printStackTrace();
            state = false;
        }
        return state;
    }

    public byte[] getReceiveData() throws IOException{
        return receiveData;
    }

    /** Description: get the send data
     *  Parameter: Non
     *  Return: send data in byte
     * */
    public byte[] getSendData() throws IOException{
        return sendData;
    }

    /** Description: set the receive data
     *  Parameter: send data in 3 bytes
     *  Return: Non
     * */
    public void setReceiveData(byte[] receiveData) throws IOException{
        this.receiveData = receiveData;
    }

    /** Description: set the send data
     *  Parameter: send data in 3 bytes
     *  Return: Non
     * */
    public void setSendData(byte[] sendData) throws IOException{
        this.sendData = sendData;
    }

    /** Description: By using the function closeSocket, we can close the socket
     *  Parameter: Non
     *  Return: Non
     * */
    public void closeSocket() throws IOException {
        clientSocket.close();
    }

    public byte[] intToByteArray(int p1,int p2,int p3) {
        return new byte[]{
                (byte) p1,
                (byte) p2,
                (byte) p3};
    }

    public void setSoTimeout(int time) throws SocketException {
        clientSocket = new DatagramSocket();
        clientSocket.setSoTimeout(time);
    }
}


