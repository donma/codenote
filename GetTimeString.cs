        /// <summary>
        ///  剛剛、分鐘前、小時前、天前、周前、個月前、年前、分鐘後、小時後、天後、周後、年後 的時間轉換
        /// </summary>
        /// <param name="dateTime"></param>
        /// <returns></returns>
        private string GetTimeString(DateTime dateTime) {

            var now = DateTime.Now;
            if (dateTime == now) { return "現在"; };
            var t1 = now;
            var t2 = now;
            var tmp = "";
            if (dateTime > now)
            {
                t2 = dateTime;
                tmp = "後";
            }
            else {
                t1 = dateTime;
                tmp = "前";
            }
            var ts = t2 - t1;
            if (ts.TotalDays >= 365) {
                return Math.Floor(ts.TotalDays/365) + "年" + tmp;
            }
            if ( (ts.TotalDays /31) >1) {
                
                return Math.Floor(ts.TotalDays / 31) + "個月" + tmp;
            }
            if (ts.TotalDays>=1)
            {

                return Math.Floor(ts.TotalDays) + "天" + tmp;
            }
            if (ts.TotalHours >= 1)
            {

                return Math.Floor(ts.TotalHours) + "小時" + tmp;
            }
            if (ts.TotalMinutes >= 1)
            {

                return Math.Floor(ts.TotalMinutes) + "分鐘" + tmp;
            }
            if (ts.Seconds >= 1)
            {

                return Math.Floor(ts.TotalSeconds) + "秒" + tmp;
            }
            return "剛剛";
        }
