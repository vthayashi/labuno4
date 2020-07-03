void serial_loop(){
  if (Serial.available()) {
    String str;
    str = Serial.readStringUntil('\r');
    str.trim();
    str.replace("\n", "");
    if (str.length() > 0) {
      str = "?" + str;
      str.replace("??", "&");
      str.replace("?", "&");
      serial_run(str);
    }
  }
}


void serial_run(String str) {
  String sa, sb;
  sa="&f8=";
  if (str.indexOf(sa) >= 0) {
    sb = str.substring(str.indexOf(sa) + sa.length()) + "&";
    sb = sb.substring(0, sb.indexOf("&"));
    float ia=sb.toFloat();
    ia=1000/ia;
    t8=(uint32_t)ia;
    if(t8<1) t8=1;
  }
  sa="&rst="; 
  if (str.indexOf(sa) >= 0) {
    sb = str.substring(str.indexOf(sa) + sa.length()) + "&";
    sb = sb.substring(0, sb.indexOf("&"));
    if(sb=="1"){
      asm volatile ("  jmp 0");
    }
  }
  sa = "&dw="; // &dw=[port],[value]
  while (str.indexOf(sa) >= 0) {
    sb = str.substring(str.indexOf(sa)) + "&";
    sb = sb.substring(0, sb.indexOf("&",1));
    str.replace(sb,"");
    sb=sb.substring(sa.length());
    uint8_t port, state;
    port = sb.substring(0, sb.indexOf(",")).toInt();
    state = sb.substring(sb.indexOf(",") + 1).toInt();
    pinMode(port, OUTPUT);
    if(state==2) if(digitalRead(port)) state=0; else state=1;
    digitalWrite(port, state);
  }
  /////&anw=[port],[value]
  sa="&aw="; 
  if (str.indexOf(sa) >= 0) {
    sb = str.substring(str.indexOf(sa) + sa.length()) + "&";
    sb = sb.substring(0, sb.indexOf("&"));
    uint16_t port, value;
    port = sb.substring(0, sb.indexOf(",")).toInt();
    value = sb.substring(sb.indexOf(",") + 1).toInt();
    analogWrite(port,value);
  }
  String pg="{";
  // json inicio
  sa="&dr="; // &dr=[porta],[porta-porta],...
  if (str.indexOf(sa) >= 0) {
    String sc;
    pg+="\"dr\":[";
    sb=str.substring( str.indexOf(sa) + sa.length() )+"&";
    sb=sb.substring(0,sb.indexOf("&"))+",";
    while(sb.indexOf(",")>=0){
      sc=sb.substring(0,sb.indexOf(","));
      sb=sb.substring(sb.indexOf(",")+1);
      //
      if(sc.indexOf("-")>0){
        uint8_t ini=sc.substring(0,sc.indexOf("-")).toInt();
        uint8_t fim=sc.substring(sc.indexOf("-")+1).toInt();
        for(uint8_t i=ini;i<=fim;i++){
            if (digitalRead(i)) pg += "1"; else pg += "0";
          pg += ",";
        }
      }else{
        uint8_t pin=sc.toInt();
          if (digitalRead(pin)) pg += "1"; else pg += "0";
        pg += ",";
      }
    }
    pg=pg.substring(0,pg.length()-1);
    Serial.print(pg+"]"); pg=",";
  }
  //
  sa="&ar="; // &ar=[porta],[porta-porta],...
  if (str.indexOf(sa) >= 0) {
    pg+="\"ar\":[";
    sb=str.substring( str.indexOf(sa) + sa.length() )+"&";
    sb=sb.substring(0,sb.indexOf("&"))+",";
    String sc;
    while(sb.indexOf(",")>=0){
      sc=sb.substring(0,sb.indexOf(","));
      sb=sb.substring(sb.indexOf(",")+1);
      if(sc.indexOf("-")>0){
        uint8_t ini=sc.substring(0,sc.indexOf("-")).toInt();
        uint8_t fim=sc.substring(sc.indexOf("-")+1).toInt();
        for(uint8_t i=ini;i<=fim;i++){
          pg += analogRead(A0+i);
          pg += ",";
        }
      }else{
        uint8_t pin=sc.toInt();
        pg += analogRead(A0+pin);
        pg += ",";
      }
    }
    pg=pg.substring(0,pg.length()-1);
    Serial.print(pg+"]"); pg=",";
  }
  ///// &w=[port],[v_trigger],[us],[num_amostras],[tr_port]
  sa="&w="; 
  if (str.indexOf(sa) >= 0){
    pg+="\"w\":[";
    uint8_t v_chn=0; uint16_t v_sinc=511; uint16_t v_us=0; uint16_t w_size=100;
    uint8_t tr_port=0;
    sb = str.substring(str.indexOf(sa) + sa.length())+"&";
    sb = sb.substring(0, sb.indexOf("&"))+",,,,,";
    v_chn=sb.substring(0, sb.indexOf(",")).toInt(); sb = sb.substring(sb.indexOf(",")+1);
    v_sinc=sb.substring(0,sb.indexOf(",")).toInt(); sb = sb.substring(sb.indexOf(",")+1);
    v_us=sb.substring(0,sb.indexOf(",")).toInt(); sb = sb.substring(sb.indexOf(",")+1);
    w_size=sb.substring(0,sb.indexOf(",")).toInt(); sb = sb.substring(sb.indexOf(",")+1);
    tr_port=sb.substring(0,sb.indexOf(",")).toInt();
    //delayMicroseconds(10);
    // fim
    if(w_size<100) w_size=100;
    uint16_t ia[w_size];
    if(v_us<167) v_us=167;
    uint32_t prev_micros;
    // trigger
    if(v_sinc>0){
      prev_micros=micros();
      uint16_t rv, prev_rv=analogRead(A0+tr_port);
      for(uint16_t i=0;i<400;i++){
        rv=analogRead(A0+tr_port);
        if(prev_rv<v_sinc) if(rv>=v_sinc) break;
        prev_rv=rv;
      }
    }
    // fim
    prev_micros=micros();
    for(uint16_t i=0; i<w_size;i++){
      ia[i]=analogRead(A0+v_chn);
      while((micros()-prev_micros)<v_us) {}
      prev_micros+=v_us;
    }
    for(uint16_t i=0; i<w_size;i++){
      pg+=String(ia[i]);
      if(i<(w_size-1)) pg+=","; else pg+="]";
      if(i%100==99){
        Serial.print(pg); pg=""; 
        Serial.flush();
      }
    }
    Serial.print(pg);
    pg=",";
  }
  // json fim
  if(pg==",") pg="";
  pg+="}\n";
  Serial.print(pg);
}
