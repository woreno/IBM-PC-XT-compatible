
p_menu_prnWifiError:
	M_tty_wrstr "RTC invalido; indisponivel"
ret

p_menu_prnWifi:
	call rtc_cfg_isOK
	jc p_menu_prnWifiError
	M_tty_wrstr "Nenhuma configurada"
ret
 

p_m_setWifiRTCError:
	M_tty_cwrstr "erro no RTC; definicoes WIFI indisponivel"
	call p_menu_errorDialog

p_menu_setWifi:
	call p_menu_clear

	mov si, offset bm09+1
	call p_menu_setTitle
	mov si, offset bm01_footer_str
	call p_menu_setFooter 
	
	mov dx, 0812h
	call tty_setCur

	;call rtc_cfg_isOK
	;jnc p_m_setWifi_RTC_Ok
	;
	;	call p_m_setWifiRTCError

	p_m_setWifi_RTC_Ok:
	
	;M_RTC_Read RTC_RAM_WIFI_ISSET
	;test al, 1
	;jnz p_m_setWifi_SettingsExists
	;
	;	jmp p_m_setWifi_askSettings

	p_m_setWifi_SettingsExists:
		M_mov es, 7C0h
		M_mov ds, cs
		xor di, di
		xor si, si

		call ds2cs
		M_tty_wrstr "SSID: "
		mov bl, RTC_RAM_WIFI_SSID
		mov cx, RTC_RAM_WIFI_SSIDSz
		call rtc_readString
		M_mov ds, es
		call ds2cs
		call tty_str
		call tty_crlf

		M_tty_wrstr "Passe: "
		mov bl, RTC_RAM_WIFI_KEY
		mov cx, RTC_RAM_WIFI_KEYSz
		call rtc_readString
		call tty_str
		call tty_crlf
		
		M_tty_wrstr "IP Servidor: "
		mov bl, RTC_RAM_WIFI_SERVER
		mov cx, 4
		call rtc_readString
		call tty_IP
		call tty_crlf
		call tty_crlf

		p_m_setWifi_askSettings:

			M_tty_cwrstr "Tecla S para modificar definicoes WIFI outra para manter"
			xor ax, ax
			INT 16h


ret


