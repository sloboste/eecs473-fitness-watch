/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.android.bluetoothlegatt;

import android.app.Activity;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.BroadcastReceiver;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.net.Uri;
import android.os.Bundle;
import android.os.IBinder;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ExpandableListView;
import android.widget.ProgressBar;
import android.widget.SimpleExpandableListAdapter;
import android.widget.TextView;
import android.widget.Toast;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.UUID;

public class DeviceControlActivity extends Activity {
    private final static String TAG = DeviceControlActivity.class.getSimpleName();

    public static final String EXTRAS_DEVICE_NAME = "DEVICE_NAME";
    public static final String EXTRAS_DEVICE_ADDRESS = "DEVICE_ADDRESS";

    private TextView mConnectionState;
    private TextView tvPed, tvBat, tvGPSlat, tvGPSlong, tvGPSs, tvGPSl; //Create TextView variables for data
    private ProgressBar pbBattery;
    private String mDeviceName;
    private String mDeviceAddress;
    private ExpandableListView mGattServicesList;
    private BluetoothLeService mBluetoothLeService;
    private ArrayList<ArrayList<BluetoothGattCharacteristic>> mGattCharacteristics =
            new ArrayList<ArrayList<BluetoothGattCharacteristic>>();
    private boolean mConnected = false;
    private BluetoothGattCharacteristic mNotifyCharacteristic;

    private final String LIST_NAME = "NAME";
    private final String LIST_UUID = "UUID";

    private double lat = 42.291;
    private double lon = -83.715;

    ArrayList<String> SavedPacketData = new ArrayList<>();

    // Code to manage Service lifecycle.
    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            mBluetoothLeService = ((BluetoothLeService.LocalBinder) service).getService();
            if (!mBluetoothLeService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth");
                finish();
            }
            // Automatically connects to the device upon successful start-up initialization.
            mBluetoothLeService.connect(mDeviceAddress);
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mBluetoothLeService = null;
        }
    };

    // Handles various events fired by the Service.
    // ACTION_GATT_CONNECTED: connected to a GATT server.
    // ACTION_GATT_DISCONNECTED: disconnected from a GATT server.
    // ACTION_GATT_SERVICES_DISCOVERED: discovered GATT services.
    // ACTION_DATA_AVAILABLE: received data from the device.  This can be a result of read
    //                        or notification operations.
    private final BroadcastReceiver mGattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (BluetoothLeService.ACTION_GATT_CONNECTED.equals(action)) {
                mConnected = true;
                updateConnectionState(R.string.connected);
                invalidateOptionsMenu();
            } else if (BluetoothLeService.ACTION_GATT_DISCONNECTED.equals(action)) {
                mConnected = false;
                updateConnectionState(R.string.disconnected);
                invalidateOptionsMenu();
                clearUI();
            } else if (BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                // Show all the supported services and characteristics on the user interface.
                displayGattServices(mBluetoothLeService.getSupportedGattServices());
            } else if (BluetoothLeService.ACTION_DATA_AVAILABLE.equals(action)) {
                displayData(intent.getStringExtra(BluetoothLeService.EXTRA_DATA));
            }
        }
    };

    // If a given GATT characteristic is selected, check for supported features.  This sample
    // demonstrates 'Read' and 'Notify' features.  See
    // http://d.android.com/reference/android/bluetooth/BluetoothGatt.html for the complete
    // list of supported characteristic features.
    private final ExpandableListView.OnChildClickListener servicesListClickListner =
            new ExpandableListView.OnChildClickListener() {
                @Override
                public boolean onChildClick(ExpandableListView parent, View v, int groupPosition,
                                            int childPosition, long id) {
                    if (mGattCharacteristics != null) {
                        final BluetoothGattCharacteristic characteristic = mGattCharacteristics.get(groupPosition).get(childPosition);
                        final int charaProp = characteristic.getProperties();

                        //if read + notify
                        if( (charaProp & (BluetoothGattCharacteristic.PROPERTY_READ|BluetoothGattCharacteristic.PROPERTY_NOTIFY)) > 0  ){
                            mBluetoothLeService.readCharacteristic(characteristic);
                            mNotifyCharacteristic = characteristic;
                            mBluetoothLeService.setCharacteristicNotification(characteristic, true);
                        }

                        //if write
                        if( (charaProp & BluetoothGattCharacteristic.PROPERTY_WRITE) >0 ){
                            byte[] value = new byte[1];
                            value[0] = 4;
                            mBluetoothLeService.writeCharacteristic(characteristic, value);
                        }

                        return true;
                    }
                    return false;
                }
    };

    //Reset all TextViews
    private void clearUI() {
        mGattServicesList.setAdapter((SimpleExpandableListAdapter) null);
        tvPed.setText(R.string.no_data);
        tvBat.setText(R.string.no_data);
        tvGPSlat.setText(R.string.no_data);
        tvGPSlong.setText(R.string.no_data);
        tvGPSs.setText(R.string.no_data);
        //tvGPSl.setText(R.string.no_data);
    }

    private void updateCharacteristic(byte packet){
        if (mGattCharacteristics != null) {
            for(ArrayList<BluetoothGattCharacteristic> service:mGattCharacteristics) {
                for (BluetoothGattCharacteristic characteristic : service) {
                    final int charaProp = characteristic.getProperties();
                    //Do a BLE write characterisic
                    if( (charaProp & BluetoothGattCharacteristic.PROPERTY_WRITE) >0 ){
                        byte[] value = new byte[1];
                        value[0] = packet;
                        mBluetoothLeService.writeCharacteristic(characteristic, value);
                    }
                }
            }
        }
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.gatt_services_characteristics);

        final Intent intent = getIntent();
        mDeviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        mDeviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);

        // Sets up UI references.
        ((TextView) findViewById(R.id.device_address)).setText(mDeviceAddress);
        mGattServicesList = (ExpandableListView) findViewById(R.id.gatt_services_list);
        mGattServicesList.setOnChildClickListener(servicesListClickListner);
        mConnectionState = (TextView) findViewById(R.id.connection_state);

        //Find TextViews from XML to manipulate them
        tvPed = (TextView) findViewById(R.id.tvPed);
        tvBat = (TextView) findViewById(R.id.tvBat);
        tvGPSlat = (TextView) findViewById(R.id.tvGPSlat);
        tvGPSlong = (TextView) findViewById(R.id.tvGPSlong);
        tvGPSs = (TextView) findViewById(R.id.tvGPSs);
        tvGPSl = (TextView) findViewById(R.id.tvGPSl);
        tvGPSl.setMovementMethod(new ScrollingMovementMethod());

        pbBattery = (ProgressBar) findViewById(R.id.pbBattery);

        getActionBar().setTitle(mDeviceName);
        getActionBar().setDisplayHomeAsUpEnabled(true);
        Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
        bindService(gattServiceIntent, mServiceConnection, BIND_AUTO_CREATE);

        updateCharacteristic(SampleGattAttributes.REQUEST_GPS_LOG);

        //Create a thread and refresh all data
        Thread t = new Thread() {
            @Override
            public void run() {
                try {
                    Thread.sleep(2000);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            // update GPS Log TextView
                            updateCharacteristic(SampleGattAttributes.REQUEST_GPS_LOG);
                        }
                    });
                    Thread.sleep(10000);
                   while (!isInterrupted()) {
                        Thread.sleep(1000);
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                // update pedometer TextView
                                updateCharacteristic(SampleGattAttributes.REQUEST_PED_STEP_COUNT);
                            }
                        });
                        Thread.sleep(1000);
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                // update battery TextView
                                updateCharacteristic(SampleGattAttributes.REQUEST_BATTERY_LEVEL);
                            }
                        });
                        Thread.sleep(1000);
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                // update GPS Data TextView
                                updateCharacteristic(SampleGattAttributes.REQUEST_GPS_DATA);
                            }
                        });
//                        Thread.sleep(1000);
//                        runOnUiThread(new Runnable() {
//                            @Override
//                            public void run() {
//                                // update GPS Log TextView
//                                updateCharacteristic(SampleGattAttributes.REQUEST_GPS_LOG);
//                            }
//                        });
                    }
                } catch (InterruptedException e) {
                }
            }
        };

        t.start();

        final Button button = (Button) findViewById(R.id.bGmaps);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Open Locaion in Google Maps
                String uri = String.format(Locale.ENGLISH, "geo:%f,%f?q=%f,%f", lat, lon, lat, lon);
                Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(uri));
                startActivity(intent);
            }
        });

        final Button button2 = (Button) findViewById(R.id.bRoute);
        button2.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Open GPS Parser: https://learn.adafruit.com/custom/ultimate-gps-parser
                String url = "https://learn.adafruit.com/custom/ultimate-gps-parser";
                Intent i = new Intent(Intent.ACTION_VIEW);
                i.setData(Uri.parse(url));
                startActivity(i);

            }
        });

    }

    @Override
    protected void onResume() {
        super.onResume();
        registerReceiver(mGattUpdateReceiver, makeGattUpdateIntentFilter());
        if (mBluetoothLeService != null) {
            final boolean result = mBluetoothLeService.connect(mDeviceAddress);
            Log.d(TAG, "Connect request result=" + result);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(mGattUpdateReceiver);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unbindService(mServiceConnection);
        mBluetoothLeService = null;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.gatt_services, menu);
        if (mConnected) {
            menu.findItem(R.id.menu_connect).setVisible(false);
            menu.findItem(R.id.menu_disconnect).setVisible(true);
        } else {
            menu.findItem(R.id.menu_connect).setVisible(true);
            menu.findItem(R.id.menu_disconnect).setVisible(false);
        }
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch(item.getItemId()) {
            case R.id.menu_connect:
                mBluetoothLeService.connect(mDeviceAddress);
                return true;
            case R.id.menu_disconnect:
                mBluetoothLeService.disconnect();
                return true;
            case android.R.id.home:
                onBackPressed();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private void updateConnectionState(final int resourceId) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mConnectionState.setText(resourceId);
            }
        });
    }

    //converts hex string to ascii string
    private String hex2s(String s){
        StringBuilder output = new StringBuilder();
        for (int i = 0; i < s.length(); i+=2) {
            String str = s.substring(i, i+2);
            output.append((char)Integer.parseInt(str, 16));
        }
        return output.toString();
    }


    // Sets TextViews to appropriate values by parsing data
    // data: a packet of data from BLE
    private void displayData(String data) {
        if (data != null) {
            //extract first byte
            byte first_byte = (byte)Integer.parseInt(data.substring(21, 23), 16);
            long unsigned_first_byte = first_byte & 0x00000000ffffffffL;

            //get packet type
            long packet_type = unsigned_first_byte & 0x7F; // & 0111 1111

            //Is it terminated?
            long terminated = (unsigned_first_byte & 0x80) >> 7; // & 1000 0000

            //get second byte which is data length
            byte second_byte = (byte)Integer.parseInt(data.substring(24, 26), 16);
            int data_length = second_byte & 0x00000000ffffffff;

            //skip gibberish packets
            if(packet_type < 0 && packet_type > 7) return;
            if(data_length < 0 && data_length > 18) return;

            String display_data = "";
            //Extract data from packet
            try {
                //display_data = data;
                System.out.println(display_data);
                display_data = data.substring(26, data.length());                                   //get data portion
                display_data = display_data.replaceAll("\\s+", "");                                 //remove whitespace
                display_data = display_data.substring(0, data_length * 2);

                //int disp_data = Integer.parseInt(display_data.substring(0, data_length * 2), 16);   //Grab only the relevent data
                //display_data = Integer.toString(disp_data & 0x00000000ffffffff);                    //convert
            }catch(Exception ex){
                System.out.println("Shit's on fire yo");
            }

            if(terminated == 1) {
                //combine Packets if needed
                if(!SavedPacketData.isEmpty()){
                    StringBuilder stringBuilder = new StringBuilder();
                    for(String packet_data:SavedPacketData){
                        //packet_data = String.format("%040x", new BigInteger(1, packet_data.getBytes(/*YOUR_CHARSET?*/)));
                        stringBuilder.append(packet_data);
                    }
                    display_data = hex2s(display_data);
                    System.out.println(display_data);
                    display_data = stringBuilder.append(display_data).toString();

                    //clear it for next use
                    SavedPacketData.clear();
                }
                if (packet_type == SampleGattAttributes.REPLY_PED_STEP_COUNT) {
                    int disp_data = Integer.parseInt(display_data, 16);   //Grab only the relevent data
                    display_data = Integer.toString(disp_data & 0x00000000ffffffff);                    //convert
                    tvPed.setText(display_data);
                } else if (packet_type == SampleGattAttributes.REPLY_BATTERY_LEVEL) {
                    int disp_data = Integer.parseInt(display_data, 16);   //Grab only the relevent data
                    display_data = Integer.toString(disp_data & 0x00000000ffffffff);                    //convert
                    //append percent sign
                    tvBat.setText(display_data+"%");
                    pbBattery.setProgress(Integer.parseInt(display_data));
                } else if (packet_type == SampleGattAttributes.REPLY_GPS_LATITUDE) {
                    display_data = hex2s(display_data);
                    tvGPSlat.setText(display_data);
                    //change d m mf to df
                    int degree = Integer.parseInt(display_data.substring(0,3));
                    double minute = Double.parseDouble(display_data.substring(4, 11));
                    char sign = display_data.charAt(display_data.length()-1);
                    lat = degree + minute/60;

                    if(sign == 'W' || sign == 'S'){
                        lat = -lat;
                    }
                } else if (packet_type == SampleGattAttributes.REPLY_GPS_LONGITUDE) {
                    display_data = hex2s(display_data);
                    tvGPSlong.setText(display_data);
                    //change d m mf to df
                    int degree = Integer.parseInt(display_data.substring(0,3));
                    double minute = Double.parseDouble(display_data.substring(4, 11));
                    char sign = display_data.charAt(display_data.length() - 1);
                    lon = degree + minute/60;

                    if(sign == 'W' || sign == 'S'){
                        lon = -lon;
                    }
                } else if (packet_type == SampleGattAttributes.REPLY_GPS_SPEED) {
                    int disp_data = Integer.parseInt(display_data, 16);   //Grab only the relevent data
                    display_data = Integer.toString(disp_data & 0x00000000ffffffff);                    //convert
                    tvGPSs.setText(display_data);
                } else if (packet_type == SampleGattAttributes.REPLY_GPS_LOG) {
                    tvGPSl.setText(display_data);
                    //System.out.println("Full Dump: " + display_data);

                    //copy data
                    ClipboardManager clipboard = (ClipboardManager) getSystemService(CLIPBOARD_SERVICE);
                    ClipData clip = ClipData.newPlainText("label", display_data);
                    clipboard.setPrimaryClip(clip);
                    Toast.makeText(getApplicationContext(), "Logged Copied to Clipboard", Toast.LENGTH_LONG).show();
                }
            }else{
                //SavedPacketData.add(display_data);
                display_data = hex2s(display_data);
                System.out.println(display_data);
                SavedPacketData.add(display_data);
            }
        }
    }

    // Demonstrates how to iterate through the supported GATT Services/Characteristics.
    // In this sample, we populate the data structure that is bound to the ExpandableListView
    // on the UI.
    private void displayGattServices(List<BluetoothGattService> gattServices) {
        if (gattServices == null) return;
        String uuid = null;
        String unknownServiceString = getResources().getString(R.string.unknown_service);
        String unknownCharaString = getResources().getString(R.string.unknown_characteristic);
        ArrayList<HashMap<String, String>> gattServiceData = new ArrayList<HashMap<String, String>>();
        ArrayList<ArrayList<HashMap<String, String>>> gattCharacteristicData
                = new ArrayList<ArrayList<HashMap<String, String>>>();
        mGattCharacteristics = new ArrayList<ArrayList<BluetoothGattCharacteristic>>();

        // Loops through available GATT Services.
        for (BluetoothGattService gattService : gattServices) {
            HashMap<String, String> currentServiceData = new HashMap<String, String>();
            uuid = gattService.getUuid().toString();
            currentServiceData.put(
                    LIST_NAME, SampleGattAttributes.lookup(uuid, unknownServiceString));
            currentServiceData.put(LIST_UUID, uuid);
            gattServiceData.add(currentServiceData);

            ArrayList<HashMap<String, String>> gattCharacteristicGroupData =
                    new ArrayList<HashMap<String, String>>();
            List<BluetoothGattCharacteristic> gattCharacteristics =
                    gattService.getCharacteristics();
            ArrayList<BluetoothGattCharacteristic> charas =
                    new ArrayList<BluetoothGattCharacteristic>();

            // Loops through available Characteristics.
            for (BluetoothGattCharacteristic gattCharacteristic : gattCharacteristics) {
                charas.add(gattCharacteristic);
                HashMap<String, String> currentCharaData = new HashMap<String, String>();
                uuid = gattCharacteristic.getUuid().toString();
                currentCharaData.put(
                        LIST_NAME, SampleGattAttributes.lookup(uuid, unknownCharaString));
                currentCharaData.put(LIST_UUID, uuid);
                gattCharacteristicGroupData.add(currentCharaData);
            }
            mGattCharacteristics.add(charas);
            gattCharacteristicData.add(gattCharacteristicGroupData);
        }

        //find read characteristic
        BluetoothGattCharacteristic characteristic = null;
        for(ArrayList<BluetoothGattCharacteristic> charalist: mGattCharacteristics){
            for(BluetoothGattCharacteristic chara: charalist){
                String ruuid = chara.getUuid().toString();
                if(ruuid.equalsIgnoreCase(SampleGattAttributes.Read)) {
                    //found read characteristic
                    characteristic = chara;
                    break;
                }
            }
        }

        //if found, send read characteristic
        if(characteristic != null){
            final int charaProp = characteristic.getProperties();
            if( (charaProp & (BluetoothGattCharacteristic.PROPERTY_READ|BluetoothGattCharacteristic.PROPERTY_NOTIFY)) > 0  ){
                mBluetoothLeService.readCharacteristic(characteristic);
                mNotifyCharacteristic = characteristic;
                mBluetoothLeService.setCharacteristicNotification(characteristic, true);
            }
        }else{
            System.out.println("READ FAILED!");
        }


        SimpleExpandableListAdapter gattServiceAdapter = new SimpleExpandableListAdapter(
                this,
                gattServiceData,
                android.R.layout.simple_expandable_list_item_2,
                new String[] {LIST_NAME, LIST_UUID},
                new int[] { android.R.id.text1, android.R.id.text2 },
                gattCharacteristicData,
                android.R.layout.simple_expandable_list_item_2,
                new String[] {LIST_NAME, LIST_UUID},
                new int[] { android.R.id.text1, android.R.id.text2 }
        );
        mGattServicesList.setAdapter(gattServiceAdapter);
    }

    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(BluetoothLeService.ACTION_DATA_AVAILABLE);
        return intentFilter;
    }
}
