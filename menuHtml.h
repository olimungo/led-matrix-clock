String html = \
"<!DOCTYPE html>" \
"<html>" \
"    <head>" \
"        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />" \
"" \
"        <style>" \
"            body {" \
"                font-family: sans-serif;" \
"                color: #0772c7;" \
"                background: #dcefff;" \
"            }" \
"" \
"            h1 {" \
"                font-size: 20px;" \
"                font-weight: 100;" \
"                margin: 0 0 10px 0;" \
"            }" \
"" \
"            button {" \
"                width: 30px;" \
"                height: 27px;" \
"                font-size: 13px;" \
"                font-weight: 600;" \
"                background: #0872c7;" \
"                border-radius: 7px;" \
"                color: white;" \
"                border: none;" \
"                margin: 0 4px;" \
"            }" \
"" \
"            .align-horizontal {" \
"                display: flex;" \
"                align-items: center;" \
"            }" \
"" \
"            .spacer-horizontal {" \
"                margin: 0 5px;" \
"            }" \
"" \
"            .spacer-vertical {" \
"                height: 30px;" \
"            }" \
"" \
"            .separator {" \
"                margin: 0 5px;" \
"                font-size: 20px;" \
"                color: orange;" \
"            }" \
"" \
"            .card {" \
"                background: white;" \
"                border: solid 1px #77a1c7;" \
"                border-radius: 5px;" \
"                margin: 20px;" \
"                padding: 10px;" \
"            }" \
"" \
"            .num {" \
"                width: 20px;" \
"                height: 25px;" \
"                font-size: 18px;" \
"                text-align: center;" \
"                padding: 0;" \
"                margin: 0;" \
"                border-radius: 2px;" \
"                border: solid 1px #ffa502;" \
"                color: #0772c7;" \
"            }" \
"" \
"            .action {" \
"                width: 60px;" \
"                height: 40px;" \
"                background: orange;" \
"                margin-right: 20px;" \
"            }" \
"" \
"            .button-large {" \
"                width: 60px;" \
"            }" \
"" \
"            .label {" \
"                width: 40px;" \
"            }" \
"        </style>" \
"    </head>" \
"" \
"    <body>" \
"        <div class=\"card\">" \
"            <h1>TIMER</h1>" \
"" \
"            <div class=\"align-horizontal\">" \
"                <div class=\"label\">MIN</div>" \
"" \
"                <div class=\"align-horizontal\">" \
"                    <button" \
"                        onclick=\"sendTimer('/action/timer/minute1/oneMore')\"" \
"                    >" \
"                        +1" \
"                    </button>" \
"                    <label id=\"minute1\" class=\"num\">%%minute1%%</label>" \
"                    <button" \
"                        onclick=\"sendTimer('/action/timer/minute1/oneLess')\"" \
"                    >" \
"                        -1" \
"                    </button>" \
"                </div>" \
"" \
"                <div class=\"spacer-horizontal\"></div>" \
"" \
"                <div class=\"align-horizontal\">" \
"                    <button" \
"                        onclick=\"sendTimer('/action/timer/minute2/oneMore')\"" \
"                    >" \
"                        +1" \
"                    </button>" \
"                    <label id=\"minute2\" class=\"num\">%%minute2%%</label>" \
"                    <button" \
"                        onclick=\"sendTimer('/action/timer/minute2/oneLess')\"" \
"                    >" \
"                        -1" \
"                    </button>" \
"                </div>" \
"            </div>" \
"" \
"            <div class=\"spacer-vertical\"></div>" \
"" \
"            <div class=\"align-horizontal\">" \
"                <div class=\"label\">SEC</div>" \
"" \
"                <div class=\"align-horizontal\">" \
"                    <button" \
"                        onclick=\"sendTimer('/action/timer/second1/oneMore')\"" \
"                    >" \
"                        +1" \
"                    </button>" \
"                    <label id=\"second1\" class=\"num\">%%second1%%</label>" \
"                    <button" \
"                        onclick=\"sendTimer('/action/timer/second1/oneLess')\"" \
"                    >" \
"                        -1" \
"                    </button>" \
"                </div>" \
"" \
"                <div class=\"spacer-horizontal\"></div>" \
"" \
"                <div class=\"align-horizontal\">" \
"                    <button" \
"                        onclick=\"sendTimer('/action/timer/second2/oneMore')\"" \
"                    >" \
"                        +1" \
"                    </button>" \
"                    <label id=\"second2\" class=\"num\">%%second2%%</label>" \
"                    <button" \
"                        onclick=\"sendTimer('/action/timer/second2/oneLess')\"" \
"                    >" \
"                        -1" \
"                    </button>" \
"                </div>" \
"            </div>" \
"" \
"            <div class=\"spacer-vertical\"></div>" \
"" \
"            <div class=\"align-horizontal\">" \
"                <button" \
"                    class=\"button-large\"" \
"                    onclick=\"sendTimer('/action/timer/minutes/fiveMore')\"" \
"                >" \
"                    +5 min" \
"                </button>" \
"                <button" \
"                    class=\"button-large\"" \
"                    onclick=\"sendTimer('/action/timer/minutes/fiveLess')\"" \
"                >" \
"                    -5 min" \
"                </button>" \
"            </div>" \
"" \
"            <div class=\"spacer-vertical\"></div>" \
"" \
"            <div class=\"align-horizontal\">" \
"                <button class=\"action\" onclick=\"send('/action/timer/start')\">" \
"                    START" \
"                </button>" \
"                <button class=\"action\" onclick=\"send('/action/timer/pause')\">" \
"                    PAUSE" \
"                </button>" \
"                <button" \
"                    class=\"action\"" \
"                    onclick=\"sendTimer('/action/timer/reset')\"" \
"                >" \
"                    RESET" \
"                </button>" \
"            </div>" \
"        </div>" \
"" \
"        <div class=\"card\">" \
"            <h1>CHRONO</h1>" \
"" \
"            <div class=\"align-horizontal\">" \
"                <button class=\"action\" onclick=\"send('/action/chrono/start')\">" \
"                    START" \
"                </button>" \
"                <button class=\"action\" onclick=\"send('/action/chrono/pause')\">" \
"                    PAUSE" \
"                </button>" \
"                <button class=\"action\" onclick=\"send('/action/chrono/reset')\">" \
"                    RESET" \
"                </button>" \
"            </div>" \
"        </div>" \
"" \
"        <div class=\"card\">" \
"            <h1>TIMEZONE SHIFT</h1>" \
"            <div class=\"align-horizontal\">" \
"                <button onclick=\"sendTimezone('/action/timezone/oneMore')\">" \
"                    +1" \
"                </button>" \
"                <label id=\"shift\" class=\"num\">%%shift%%</label>" \
"                <button onclick=\"sendTimezone('/action/timezone/oneLess')\">" \
"                    -1" \
"                </button>" \
"            </div>" \
"        </div>" \
"" \
"        <script>" \
"            function sendTimer(action) {" \
"                const Http = sendHttpRequest(action);" \
"" \
"                Http.onreadystatechange = event => {" \
"                    if (Http.responseText) {" \
"                        const result = JSON.parse(Http.responseText);" \
"" \
"                        setTimerValues(" \
"                            result.minute1," \
"                            result.minute2," \
"                            result.second1," \
"                            result.second2" \
"                        );" \
"                    }" \
"                };" \
"            }" \
"" \
"            function sendTimezone(action) {" \
"                const Http = sendHttpRequest(action);" \
"" \
"                Http.onreadystatechange = event => {" \
"                    if (Http.responseText) {" \
"                        const result = JSON.parse(Http.responseText);" \
"                        const shiftElem = document.getElementById('shift');" \
"" \
"                        shiftElem.textContent = result.shift;" \
"                    }" \
"                };" \
"            }" \
"" \
"            function send(action) {" \
"                const Http = sendHttpRequest(action);" \
"" \
"                Http.onreadystatechange = event => {" \
"                    console.log(Http.responseText);" \
"                };" \
"            }" \
"" \
"            function sendHttpRequest(action) {" \
"                const Http = new XMLHttpRequest();" \
"                Http.open('GET', action);" \
"                Http.send();" \
"" \
"                return Http;" \
"            }" \
"" \
"            function setTimerValues(minute1, minute2, second1, second2) {" \
"                const minute1Elem = document.getElementById('minute1');" \
"                const minute2Elem = document.getElementById('minute2');" \
"                const second1Elem = document.getElementById('second1');" \
"                const second2Elem = document.getElementById('second2');" \
"" \
"                minute1Elem.textContent = minute1;" \
"                minute2Elem.textContent = minute2;" \
"                second1Elem.textContent = second1;" \
"                second2Elem.textContent = second2;" \
"            }" \
"        </script>" \
"    </body>" \
"</html>";
