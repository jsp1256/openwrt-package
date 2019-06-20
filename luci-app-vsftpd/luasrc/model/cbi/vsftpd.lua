--[[
Copyright (C) 2019 Jianpeng Xiang (1505020109@mail.hnust.edu.cn)

This is free software, licensed under the GNU General Public License v3.
]]--

require("luci.sys")
require("luci.util")
--得到Map对象，并初始化。参一：指定cbi文件，参二：设置标题，参三：设置标题下的注释
m=Map("vsftpd",translate("FTP Service"),translate("Use this page, you can share your file under web via ftp."))
--判断vsftpd程序是否运行
m:section(SimpleSection).template="vsftpd_status"
s=m:section(TypedSection,"vsftpd",translate("vsFTPd Settings"))
s.addremove=false
s.anonymous=true

s:tab("general",translate("Global"))
s:tab("localuser",translate("Local User"))
s:tab("anonymous",translate("Anonymous"))
s:tab("encryption",translate("Encryption Option"))
s:tab("template",translate("Template"))


enable=s:taboption("general",Flag,"enabled",translate("Enabled"))
enable.rmempty=false
listen_ipv6=s:taboption("general",Flag,"listen_ipv6",translate("Enable IPv6"))
listen_ipv6.rmempty=false
banner=s:taboption("general",Value,"ftpd_banner",translate("FTP banner"))
banner.rmempty=true
banner.placeholder="OpenWRT Router Embd FTP service."
max_clients=s:taboption("general",Value,"max_clients",translate("Max number of clients"))
max_clients:value("5","5")
max_clients:value("10","10")
max_clients:value("15","15")
max_clients:value("20","20")
max_clients.placeholder="10"
max_clients.datatype="range(1,100)"
max_clients.default=10
max_per_ip=s:taboption("general",Value,"max_per_ip",translate("Max threads of per ip"))
max_per_ip:value("1","1")
max_per_ip:value("2","2")
max_per_ip:value("3","3")
max_per_ip:value("4","4")
max_per_ip:value("5","5")
max_per_ip.placeholder="5"
max_per_ip.datatype="range(1,10)"
max_per_ip.default=5
ascii=s:taboption("general",ListValue,"ascii",translate("ASCII availabled"))
ascii:value("both",translate("Both Download and Upload"))
ascii:value("download",translate("Download only"))
ascii:value("upload",translate("Upload only"))
ascii:value("none",translate("None"))
connect_from_port_20=s:taboption("general",Flag,"connect_from_port_20",translate("Data Port using 20"))
connect_from_port_20.rmempty=false
pasv_enable=s:taboption("general",Flag,"pasv_enable",translate("Enable Pasv Mode"))
pasv_enable.rmempty=false
pasv_min_port=s:taboption("general",Value,"pasv_min_port",translate("Pasv Min Port"))
pasv_min_port.rmempty=true
pasv_min_port.placeholder="1024"
pasv_min_port.default="1024"
pasv_min_port.datatype="range(1024,65535)"
pasv_min_port:depends("pasv_enable",1)
pasv_max_port=s:taboption("general",Value,"pasv_max_port",translate("Pasv Max Port"))
pasv_max_port.rmempty=true
pasv_max_port.placeholder="65535"
pasv_max_port.default="65535"
pasv_max_port.datatype="range(1024,65535)"
pasv_max_port:depends("pasv_enable",1)
async_abor=s:taboption("general",Flag,"async_abor_enable",translate("Accept Special Cmd"))
async_abor.rmempty=false
ls_recurse=s:taboption("general",Flag,"ls_recurse_enable",translate("Allow exhaustive listing"))
ls_recurse.rmempty=false
dirmessage=s:taboption("general",Flag,"dirmessage_enable",translate("Enable DIR Message"))
dirmessage.rmempty=false
idle_timeout=s:taboption("general",Value,"idle_session_timeout",translate("Idle timeout"))
idle_timeout.default=600
idle_timeout.placeholder="600"
data_connection_timeout=s:taboption("general",Value,"data_connection_timeout",translate("Transfer timeout"))
data_connection_timeout.default=200
data_connection_timeout.placeholder="200"
xferlog_enable=s:taboption("general",Flag,"xferlog_enable",translate("Enable FTP Logging"))
xferlog_enable.rmempty=false
xferlog_file=s:taboption("general",Value,"xferlog_file",translate("Log Path"))
xferlog_file.default="/var/log/vsftpd.log"
xferlog_file.rmempty=false
xferlog_file.placeholder="/var/log/vsftpd.log"
xferlog_file:depends("xferlog_enable",1)

local_enable=s:taboption("localuser",Flag,"local_enable",translate("Allow local member"))
local_enable.rmempty=false
local_write=s:taboption("localuser",Flag,"write_enable",translate("Member can write"))
local_write.rmempty=false
local_write:depends("local_enable",1)
local_chown=s:taboption("localuser",Flag,"chown_uploads",translate("Allow change permissions"))
local_chown.rmempty=false
local_chown:depends("local_enable",1)
local_chroot=s:taboption("localuser",Flag,"chroot_local_user",translate("Enable chroot"))
local_chroot.rmempty=false
local_chroot:depends("local_enable",1)
local_max_rate=s:taboption("localuser",Value,"local_max_rate",translate("Speed limit"),translate("In KB/s. 0 means unlimited."))
local_max_rate:value("0",translate("Unlimit"))
local_max_rate:value("64","64")
local_max_rate:value("128","128")
local_max_rate:value("256","256")
local_max_rate:value("512","512")
local_max_rate:value("1024","1024")
local_max_rate:value("2048","2048")
local_max_rate.placeholder="0"
local_max_rate.datatype="range(0,128000)"
local_max_rate.rmempty=true
local_max_rate:depends("local_enable",1)
local_umask=s:taboption("localuser",Value,"local_umask",translate("uMask for new uploads"),translate("The format for number likes ###, first bit for the file's Master. second bit for the Groups which Master have joined, last bit for other people. Every bit's value from 0 to 7: 4 means read, 2 means write, 1 means execute. The value of a bit is the sigma of above listed value. When a file created, the default value is 777\(that means everyone can read write and execute the file,\) and the vsftpd will deduct the value which you set from default value."))
local_umask.default="022"
local_umask:value("000","000")
local_umask:value("022","022")
local_umask:value("027","027")
local_umask.placeholder="000"
local_umask.datatype="range(0,777)"
local_umask.rmempty=true
local_umask:depends("local_enable",1)

anon_enabled=s:taboption("anonymous",Flag,"anonymous_enable",translate("Allow anonymous"))
anon_enabled.rmempty=false
anon_upload=s:taboption("anonymous",Flag,"anon_upload_enable",translate("Anonymous can upload"),translate("Uncomment this to allow the anonymous FTP user to upload files. This only has an effect if the above global write enable is activated. Also, you will obviously need to create a directory writable by the FTP user"))
anon_upload.rmempty=false
anon_upload:depends("anonymous_enable",1)
anon_upload:depends("write_enable",1)
anon_mkdir=s:taboption("anonymous",Flag,"anon_mkdir_write_enable",translate("Anonymous can create folder"))
anon_mkdir.rmempty=false
anon_mkdir:depends("anonymous_enable",1)
anon_mkdir:depends("write_enable",1)
anon_root=s:taboption("anonymous",Value,"anon_root",translate("Anonymous root"))
anon_root.default="/mnt"
anon_root.rmempty=false
anon_max_rate=s:taboption("anonymous",Value,"anon_max_rate",translate("Speed limit"),translate("In KB/s. 0 means unlimited."))
anon_max_rate:value("0","0")
anon_max_rate:value("64","64")
anon_max_rate:value("128","128")
anon_max_rate:value("256","256")
anon_max_rate:value("512","512")
anon_max_rate:value("1024","1024")
anon_max_rate:value("2048","2048")
anon_max_rate.placeholder="0"
anon_max_rate.datatype="range(0,128000)"
anon_max_rate.rmempty=true
anon_max_rate:depends("anonymous_enable",1)
chown_username=s:taboption("anonymous",ListValue,"chown_username",translate("Chown User"),translate("This is the name of the user who is given ownership of anonymously uploaded files. This option is only relevant if another option,chown_uploads,is set"))
chown_username:depends("anonymous_enable",1)
chown_username:depends("chown_uploads",1)
for _, list_user in luci.util.vspairs(luci.util.split(luci.sys.exec("cat /etc/passwd | cut -f 1 -d:"))) do
    chown_username:value(list_user)
end

ssl_enable=s:taboption("encryption",Flag,"ssl_enable",translate("Enable SSL encryption"))
ssl_enable.rmempty=false
allow_anon_ssl=s:taboption("encryption",Flag,"allow_anon_ssl",translate("Allow anonymous use SSL encryption"))
allow_anon_ssl.rmempty=false
allow_anon_ssl:depends("ssl_enable",1)
rsa_cert_file=s:taboption("encryption",Value,"rsa_cert_file",translate("RSA certificate file path"),translate("This option specifies the location of the RSA certificate to use for SSL encrypted connections."))
rsa_cert_file.placeholder="/etc/certificate.pem"
rsa_cert_file.default="/etc/certificate.pem"
rsa_cert_file:depends("ssl_enable",1)
rsa_private_key_file=s:taboption("encryption",Value,"rsa_private_key_file",translate("RSA private key file path"),translate("This option specifies the location of the RSA private key to use for SSL encrypted connections. If this option is not set, the private key is expected to be in the same file as the certificate"))
rsa_private_key_file:depends("ssl_enable",1)
rsa_private_key_file.placeholder="/etc/certificate.key"
rsa_private_key_file.default="/etc/certificate.key"
ssl_ciphers=s:taboption("encryption",Value,"ssl_ciphers",translate("SSL ciphers"))
for _, list_ssl_ciphers in luci.util.vspairs(luci.util.split(luci.sys.exec("openssl ciphers -v | awk '{print $1}'"))) do
	if(string.len(list_ssl_ciphers) > 0)
	then
		ssl_ciphers:value(list_ssl_ciphers)
	end
end
ssl_ciphers.default="DES-CBC3-SHA"
ssl_ciphers:depends("ssl_enable",1)
implicit_ssl=s:taboption("encryption",Flag,"implicit_ssl",translate("Implicit SSL enable"))
implicit_ssl.rmempty=false
implicit_ssl:depends("ssl_enable",1)
require_ssl_reuse=s:taboption("encryption",Flag,"require_ssl_reuse",translate("Require SSL reuse"),translate("If set to yes, all SSL data connections are required to exhibit SSL session reuse (which proves that they know the same master secret as the control channel). Although this is a secure default, it may break many FTP clients,so you may want to disable it"))
require_ssl_reuse.rmempty=false
require_ssl_reuse:depends("ssl_enable",1)

tmpl=s:taboption("template",Value,"_tmpl","",translate("Here,you can edit the template of config file"))
tmpl.template = "cbi/tvalue"
tmpl.rows=20

function tmpl.cfgvalue(self, section)
	return nixio.fs.readfile("/etc/vsftpd.conf.template")
end

function tmpl.write(self, section, value)
	value = value:gsub("\r\n?", "\n")
	nixio.fs.writefile("/etc/vsftpd.conf.template", value)
end


return m
