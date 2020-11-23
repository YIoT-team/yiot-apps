package io.kutashenko.wifi;

import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiConfiguration;

import android.content.Context;
import android.content.BroadcastReceiver;

import android.app.Activity;
import android.util.Log;
import android.net.ConnectivityManager;
import android.os.Bundle;

import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONException;

import java.util.List;

public class Wifi {
    private static final String  TAG = "WiFi Activity";

   public static String enumWifi(Context ctx) {
        try {
            WifiManager w = (WifiManager) ctx.getSystemService(Context.WIFI_SERVICE);
            List<ScanResult> results = w.getScanResults();
            final int cnt = results.size();
            Log.v(TAG, "Wifi Scan Results Count: " + cnt);

            JSONObject jsonObject = new JSONObject();
            JSONArray array = new JSONArray();
            for (ScanResult res : results) {
                Log.v(TAG, "SSID  =  " + res.SSID + " : " + Integer.toString(res.level));
                JSONObject jsonWifi = new JSONObject();
                jsonWifi.put("ssid", res.SSID);
                jsonWifi.put("rssi", res.level);
                array.put(jsonWifi);
            }
            jsonObject.put("wifi", array);

            w.startScan();

            return jsonObject.toString();
        } catch (JSONException e) {
            return "";
        }
    }
}
