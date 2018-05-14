using System;
using System.Collections.Specialized;
using System.Net;
using System.Text;


namespace TestLineNotify
{
    public partial class index : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            //Step0: regist LINT NOTIFIY from : https://notify-bot.line.me


            //Step 1 : 
            //Tranfer user to your APP Notification to Allow something : 
            //https://notify-bot.line.me/oauth/authorize?
            //response_type =code
            //&client_id=YOUR_CLIENT_ID
            //&redirect_uri=YOUR_CALLBACK_URL
            //&scope=notify
            //&state=state


            //Step 2 : 
            //After your user authorize LINE will regirect to your redirect_uri from Step1
            //in your redirect_uri , you will get querystring value named "code" 
            //like thie : http://localhost:62670/risiv.aspx?code=aTONuztevWAOtgDzu8qzc9xxxx&state=checksum


            //Step3 : 
            //Get Token by code.
            //https://notify-bot.line.me/oauth/token

            //sample code
            WebClient wc = new WebClient();
            string targetAddress = "https://notify-bot.line.me/oauth/token";
            wc.Encoding = Encoding.UTF8;
            wc.Headers[HttpRequestHeader.ContentType] = "application/x-www-form-urlencoded";
            NameValueCollection nc = new NameValueCollection();
            nc["grant_type"] = "authorization_code";
            nc["code"] = "aTONuztevWAOtgDzu8qzc9xxxx"; //the value from Step 2
            nc["redirect_uri"] = "http://localhost:62670/risiv.aspx"; // the url your fill in Step 1
            nc["client_id"] = "2pfCLBdsaddkILdcXea9UR7q"; // get from https://notify-bot.line.me
            nc["client_secret"] = "ML0uxbg6IGenrhpOFhhHVDze6OnP063Pju6aDkdcbgu"; //get from https://notify-bot.line.me
            byte[] bResult = wc.UploadValues(targetAddress, nc);
            string result = Encoding.UTF8.GetString(bResult);
            Response.Write(result);

            //LINE server response like this : 
            //{"status":200,"message":"access_token is issued","access_token":"v8kF7s1qfDyOexIgQIhgUmN6qO56zOAcbOB0EjogWN1M"}


            //Step 4 : 
            //Send Message to User by token
            WebClient wc4 = new WebClient();
            string targetAddress4 = "https://notify-api.line.me/api/notify";
            wc4.Encoding = Encoding.UTF8;
            wc4.Headers[HttpRequestHeader.ContentType] = "application/x-www-form-urlencoded";
            wc4.Headers["Authorization"] = "Bearer v8kF7s1qfDyOeIgQsdsdsIhgUmN6qO56zOAcbOB0EjogWN1M"; 
            //Bearer TOKEN_FROM_STEP3
            NameValueCollection nc4 = new NameValueCollection();
            nc4["message"] = "TEST SOME MESSAGE";
            
            byte[] bResult4 = wc.UploadValues(targetAddress4, nc4);
            string result4 = Encoding.UTF8.GetString(bResult4);
            Response.Write(result4);
        }
    }
}
