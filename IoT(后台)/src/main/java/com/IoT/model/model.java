package com.IoT.model;

public class model {
	
	private int redsensor;
	
	private int rain;  //下雨时间
	
	private int threshold;
	
	public int getRain() {
		return rain;
	}
	public void setRain(int rain) {
		this.rain= rain;
	}
	
	public int getRedsensor() {
		return redsensor;
	}
	public void setRedsensor(int redsensor) {
		this.redsensor= redsensor;
	}
	
	public int getThreshold() {
		return threshold;
	}
	public void setThreshold(int threshold) {
		this.threshold= threshold;
	}
}
