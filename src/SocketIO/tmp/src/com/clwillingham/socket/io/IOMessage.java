package com.clwillingham.socket.io;

public class IOMessage {
	public static final int DISCONNECT = 0;
	public static final int CONNECT = 1;
	public static final int HEARTBEAT = 2;
	public static final int MESSAGE = 3;
	public static final int JSONMSG = 4;
	public static final int EVENT = 5;
	public static final int ACK = 6;
	public static final int ERROR = 7;
	
	private int type;
	private int id = -1;
	private String endpoint = "";
	private String messageData;
	
	public IOMessage(int type, int id, String endpoint, String data){
		this.type = type;
		this.id = id;
		this.endpoint = endpoint;
		this.messageData = data;
	}
	
	public IOMessage(int type, String endpoint, String data) {
		this.type = type;
		this.endpoint = endpoint;
		this.messageData = data;
	}
	
	public IOMessage(){
		
	}
	
	
	public static IOMessage parseMsg(String message){
		String[] content = message.split(":", 4);
		IOMessage msg = new IOMessage();
		msg.setType(Integer.parseInt(content[0]));
		
		if(message.endsWith("::")){
			msg.setId(-1);
			msg.setMessageData("");
			msg.setEndpoint("");
			return msg;
		}
		System.out.println(content[1]);
		if(!content[1].equals("")){
			msg.setId(Integer.parseInt(content[1]));
		}
		if(!content[2].equals("")){
			msg.setEndpoint(content[2]);
		}
		if(content.length > 3 && !content[3].equals("")){
			msg.setMessageData(content[3]);
		}
		return msg;
	}
	
	public String toString(){
		if(id == -1 && endpoint.equals("") && messageData.equals("")){
			return type+"::";
		}
		else if(id == -1 && endpoint.equals("")){
			return type+":::"+messageData;
		}
		else if(id > -1){
			return type+":"+id+":"+endpoint+":"+messageData;
		}
		else{
			return type+"::"+endpoint+":"+messageData;
		}
	}


	public void setType(int type) {
		this.type = type;
	}


	public int getType() {
		return type;
	}


	public void setId(int id) {
		this.id = id;
	}


	public int getId() {
		return id;
	}


	public void setEndpoint(String endpoint) {
		this.endpoint = endpoint;
	}


	public String getEndpoint() {
		return endpoint;
	}


	public void setMessageData(String messageData) {
		this.messageData = messageData;
	}


	public String getMessageData() {
		return messageData;
	}
	

}
