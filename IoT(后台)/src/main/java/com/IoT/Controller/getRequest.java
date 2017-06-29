package com.IoT.Controller;

import java.io.IOException;
import java.io.PrintWriter;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import javax.servlet.http.HttpServletResponse;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;

import com.IoT.DAO.DAO;
import net.sf.json.JSONObject;

@Controller
@RequestMapping("get")
public class getRequest {
	@Autowired
	private DAO md;
	
	@RequestMapping("")
	void get(HttpServletResponse response) throws IOException
	{
		response.setCharacterEncoding("UTF-8");
		PrintWriter out = response.getWriter();
		JSONObject res = new JSONObject();
		res.put("data", md.getData());
		out.print(res.toString());
		out.flush();
		out.close();
	}
	
	
	
}
