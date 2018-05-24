
// Ctrl + l : lock screen
// Alt  + l : unlock
// Required : 
// 1.JQuery 
// 2.HoldOn.js : https://sdkcarlos.github.io/sites/holdon.html


$(document).keydown(function (e) {
                if (event.key == "l" && event.ctrlKey) {
                    event.preventDefault();
                    var options = {
                        theme: "sk-dot",
                        message: '螢幕已經被鎖定...',
                        backgroundColor: "black",
                        textColor: "white"
                    };

                    HoldOn.open(options);
                }
                if (event.key == "l" && event.altKey) {
                    event.preventDefault();
                    HoldOn.close();
                }
});
