package com.IoT.DAO;

import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import javax.servlet.SessionTrackingMode;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Repository;
import com.IoT.model.model;
import com.mysql.jdbc.Statement;

import net.sf.json.JSONArray;

@Repository
public class DAO {
	@Autowired
	private JdbcTemplate jdb;
	
	public String getData()
	{
		String sql="select * from iotdata;";
		Object[] para = new Object[]{1};
		
		List<model> lc = jdb.query(sql, new RowMapper<model>() {

			public model mapRow(ResultSet res, int arg1) throws SQLException
			{
				model c = new model();
				c.setRain(res.getInt("rain"));
				c.setRedsensor(res.getInt("redsensor"));
				c.setThreshold(res.getInt("threshold"));
				return c;
			}
			
		});
		return JSONArray.fromObject(lc).toString();
	}
	
	public void setRain() {
		String sql = "select rain from iotdata where id= 1;";
		List<model> m= jdb.query(sql, new RowMapper<model>(){
			public model mapRow(ResultSet res, int arg1) throws SQLException
			{
				model m = new model();
				m.setRain(res.getInt("rain"));
				return m;
			}
		});
		int cur_rain= m.get(0).getRain();
		if (cur_rain == 0) {
			String stmt = "update iotdata set rain= ? " +"where id= 1;";
			jdb.update(stmt, new Object[]{1});
		} else {
			String stmt = "update iotdata set rain= ? " +"where id= 1;";
			jdb.update(stmt, new Object[]{0});
		}
		
	}
	
	public void setRedsensor() {
		String sql = "select redsensor from iotdata where id= 1;";
		List<model> m= jdb.query(sql, new RowMapper<model>(){
			public model mapRow(ResultSet res, int arg1) throws SQLException
			{
				model m = new model();
				m.setRedsensor(res.getInt("redsensor"));
				return m;
			}
		});
		int cur_red= m.get(0).getRedsensor();
		if (cur_red == 0) {
			String stmt = "update iotdata set redsensor= ? " +"where id= 1;";
			jdb.update(stmt, new Object[]{1});
		} else {
			String stmt = "update iotdata set redsensor= ? " +"where id= 1;";
			jdb.update(stmt, new Object[]{0});
		}
		
	}
	
	public void setThreshold(int threshold) {
		String stmt = "update iotdata set threshold= ? " +"where id= 1;";
		jdb.update(stmt, new Object[]{threshold});
	}
}
