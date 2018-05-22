  WebClient wc4 = new WebClient();
  string targetAddress4 = "https://rest.nexmo.com/sms/json";
  wc4.Encoding = Encoding.UTF8;
  wc4.Headers[HttpRequestHeader.ContentType] = "application/x-www-form-urlencoded";
  NameValueCollection nc4 = new NameValueCollection();
  nc4["api_key"] = YOUR_API_KEY;
  nc4["api_secret"] = YOUR_API_SECRET;
  nc4["to"] = mobile;
  //必填
  nc4["from"] = "no2studio";
  nc4["text"] = YOUR_CONTENT;
  //如果你要傳送中文，這一個地方必加，不然會出現很多 ?
  nc4["type"] = "unicode";
  byte[] bResult4 = wc4.UploadValues(targetAddress4, nc4);
  string result = Encoding.Default.GetString(bResult4);
  Console.Write(result);
