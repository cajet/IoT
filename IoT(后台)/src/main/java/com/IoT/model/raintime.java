package com.IoT.model;

public class raintime {
	
	private static long begintime;
	
	private static long endtime;
	
	private static boolean flag= true;
	
	private static int millisecond= 0;
	
	public void setBeginTime(long bt) {
		this.begintime= bt;
	}
	public long getBeginTime() {
		return begintime;
	}
	
	public void setEndTime(long et) {
		this.endtime= et;
	}
	public long getEndTime() {
		return endtime;
	}
	public void setFlag(boolean flag) {
		this.flag= flag;
	}
	public boolean getFlag() {
		return flag;
	}
	public void setMillisec(int ms) {
		this.millisecond= ms;
	}
	public int getMillisec() {
		return millisecond;
	}
}
