package com.robotxy.fmgb.xandroidy;

import android.content.Context;
import android.net.ConnectivityManager;
import android.os.AsyncTask;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

/**
 * Created by fmgb on 3/13/16.
 */
public class Network extends AsyncTask<String,Void,String>{

    String URLbase = "http://192.168.1.137:5000";
    Network()
    {


    }

    private String readStream(InputStream is) {
        try {
            ByteArrayOutputStream bo = new ByteArrayOutputStream();
            int i = is.read();
            while(i != -1) {
                bo.write(i);
                i = is.read();
            }
            return bo.toString();
        } catch (IOException e) {
            return "";
        }
    }

    @Override
    protected String doInBackground(String... urls) {
        String result = "";
        try{
            URL url = new URL(URLbase + urls[0]);
            System.out.println("URL" + url);
            HttpURLConnection con = (HttpURLConnection) url.openConnection();
            int statusCode = con.getResponseCode();
            if(statusCode != 200){
                System.err.println("El recurso no está disponible");
            }
            else
            {
                InputStream in = new BufferedInputStream(con.getInputStream());
                result = readStream(in);
                //System.out.println(readStream(in));
            }
        } catch(Exception e)
        {
            System.err.println(e);
            return null;
        }
        return result;
    }

    @Override
    protected void onPostExecute(String cantidadProcesados) {
        System.out.println("He terminado");
    }

    Network(String dir, String port)
    {
        URLbase = "http://" + dir + ":" + port;
        System.out.println(URLbase);
    }

    boolean checkNetwork()
    {
       // ConnectivityManager connMgr = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
        return true;
    }

    void get(String element) throws IOException {
       // doInBackground([this.URLbase])
        /*URL url = new URL(this.URLbase + element);
        System.out.println(url);
        HttpURLConnection urlConnection = (HttpURLConnection) url.openConnection();
        System.out.println("HOLA");
        try {
            InputStream in = new BufferedInputStream(urlConnection.getInputStream());
            System.out.println("HOLA");
            System.out.println(in);
            // Acciones a realizar con el flujo de datos
        }finally {
                urlConnection.disconnect();
            }*/
    }
}
