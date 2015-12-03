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

import java.util.HashMap;

/**
 * This class includes a small subset of standard GATT attributes for demonstration purposes.
 */
public class SampleGattAttributes {
    private static HashMap<String, String> attributes = new HashMap();

    //Service
    public static String Eir_Watch = "78c10001-801a-11e5-8bcf-feff819cdc9f";

    //chara
    public static String Write = "78c10002-801a-11e5-8bcf-feff819cdc9f";
    public static String Read = "78c10003-801a-11e5-8bcf-feff819cdc9f";

    //desc
    public static String CLIENT_CHARACTERISTIC_CONFIG = "00002902-0000-1000-8000-00805f9b34fb";

    //data request constants
    public static byte    REQUEST_PED_STEP_COUNT  = 01;
    public static byte    REQUEST_GPS_DATA        = 02;
    public static byte    REQUEST_GPS_LOG         = 03;
    public static byte    REQUEST_BATTERY_LEVEL   = 04;
    public static byte    REQUEST_HEART_RATE      = 05;

    //data reply constants
    public static byte    REPLY_PED_STEP_COUNT    = 01;
    public static byte    REPLY_GPS_LATITUDE      = 02;
    public static byte    REPLY_GPS_LONGITUDE     = 03;
    public static byte    REPLY_GPS_SPEED         = 04;
    public static byte    REPLY_GPS_LOG           = 05;
    public static byte    REPLY_BATTERY_LEVEL     = 06;
    public static byte    REPLY_HEART_RATE        = 07;

    static {
        // Sample Services.
        attributes.put("78c10001-801a-11e5-8bcf-feff819cdc9f", "Eir Watch Service");

        // Sample Characteristics.
        attributes.put(Write, "Write");
        attributes.put(Read, "Read");
    }

    public static String lookup(String uuid, String defaultName) {
        String name = attributes.get(uuid);
        return name == null ? defaultName : name;
    }
}
