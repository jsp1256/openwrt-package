--[[
Copyright (C) 2019 Jianpeng Xiang (1505020109@mail.hnust.edu.cn)

This is free software, licensed under the GNU General Public License v3.
]]--

module("luci.controller.vsftpd",package.seeall)

function index() 
	require("luci.i18n")
	luci.i18n.loadc("vsftpd")
	if not nixio.fs.access("/etc/config/vsftpd") then
		return
	end
	
	local page = entry({"admin","services","vsftpd"},cbi("vsftpd"),_("FTP Service"))
	page.i18n="vsftpd"
	page.dependent=true
	
	entry({"admin","services","vsftpd","status"}, call("connection_status"))
end

local function status_report()
return luci.sys.call("pidof vsftpd > /dev/null") == 0
end

function connection_status()
  luci.http.prepare_content("application/json")
  luci.http.write_json({run_state=status_report()})
end
