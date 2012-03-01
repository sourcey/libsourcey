package com.clwillingham.socket.io;

import org.json.JSONObject;

public interface MessageCallback {
	public void on(String event, JSONObject... data);
	public void onMessage(String message);
	public void onMessage(JSONObject json);
	public void onConnect();
	public void onDisconnect();
	public void onConnectFailure();
}
