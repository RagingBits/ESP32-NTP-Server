#ifndef WEBPAGE_H
#define WEBPAGE_H

const char INDEX_HTML[] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Dual Clock Dashboard</title>
    <style>
        * { box-sizing: border-box; margin: 0; padding: 0; }
        
        body, html { 
            height: 100%; 
            font-family: 'Orbitron', 'Segoe UI', Arial, sans-serif; 
            color: #ffffff; 
            background-color: #050508;
            overflow: hidden;
        }
        
        body {
            background-image: url('/background.jpg');
            background-size: auto;              
            background-repeat: repeat;          /* Infinite pattern tiling enabled globally */
            background-position: top left;
            background-attachment: fixed;       
            display: flex;
            flex-direction: column;
        }
        
        /* Top Half Layout Frame */
        .top-section {
            height: 50vh;
            display: flex;
            background: linear-gradient(180deg, rgba(255, 0, 0, 0.03) 0%, rgba(0, 0, 0, 0.6) 100%);
            border-bottom: 2px solid rgba(255, 0, 0, 0.4);
            box-shadow: 0 4px 25px rgba(255, 0, 0, 0.25);
            position: relative;
        }
        
        .clock-panel {
            flex: 1;
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
            padding: 10px;
        }
        
        .clock-panel:first-child {
            border-right: 1px solid rgba(255, 0, 0, 0.3);
        }
        
        /* 5X LARGER CLOCK TITLE WITH POSITIVE MARGIN FIXED */
        .clock-title { 
            font-size: 4.25rem;       
            text-transform: uppercase; 
            letter-spacing: 6px;       
            color: #ff0000; 
            margin-bottom: 25px;       /* Positive margin pushes time digits DOWN */
            padding-bottom: 10px;      
            font-weight: bold;
            text-shadow: 0 0 12px rgba(255, 0, 0, 0.7);
            opacity: 0.9;
        }
        
        /* 3X MASSIVE TIME TEXT DISPLAY */
        .time-display { 
            font-size: 6.6rem; 
            font-weight: 900; 
            line-height: 1.0;
            letter-spacing: 2px;
            color: #ffffff;
            text-shadow: 0 0 15px rgba(255, 255, 255, 0.3), 0 0 35px rgba(255, 0, 0, 0.5);
        }
        
        /* EXACTLY HALF TIME SIZE DIRECTLY UNDERNEATH */
        .date-display { 
            font-size: 3.3rem; 
            font-weight: 700;
            line-height: 1.0;
            letter-spacing: 1px;
            color: rgba(255, 255, 255, 0.9);
            text-align: center;
            text-shadow: 0 0 12px rgba(255, 0, 0, 0.3);
            margin-top: 10px;
        }

        /* Unified Network & Atomic Telemetry Strip */
        .telemetry-bar {
            position: absolute; 
            bottom: 12px; 
            left: 25px; 
            display: flex; 
            gap: 25px; 
            font-size: 0.85rem; 
            color: #ff0000; 
            opacity: 0.65; 
            letter-spacing: 2px; 
            font-weight: bold;
        }

        /* Bottom Half Layout Frame */
        .bottom-section {
            height: 50vh;
            display: flex;
            justify-content: center;
            align-items: center;
            position: relative; 
            background: rgba(0, 0, 0, 0.3); /* Transparent overlay allowing the tiled background to pass through */
        }

        /* LOGO RENDERING IN UNCONSTRAINED NATURAL SIZE ANCHORED IN UPPER-LEFT SCREEN CORNER */
        .screen-logo {
            position: absolute;
            top: 25px;            
            left: 25px;           
            width: auto;          /* Removed hard restrictions to match source dimensions */
            height: auto;         
            opacity: 0.9;
            filter: drop-shadow(0 0 8px rgba(255, 0, 0, 0.6));
            z-index: 10;          
        }
        
        form {
            background: rgba(5, 5, 5, 0.95);
            padding: 30px;
            border-radius: 6px;
            width: 90%;
            max-width: 420px;
            box-shadow: 0 0 30px rgba(255, 0, 0, 0.25);
            border: 1px solid #ff0000;
            position: relative;
        }
        
        form::before {
            content: '';
            position: absolute;
            top: -1px; left: -1px; width: 15px; height: 15px;
            border-top: 3px solid #ffffff; border-left: 3px solid #ffffff;
        }

        .form-group { margin-bottom: 16px; }
        
        label { 
            display: block; 
            font-size: 0.8rem; 
            margin-bottom: 6px; 
            font-weight: 700; 
            text-transform: uppercase; 
            letter-spacing: 2px;
            color: #ff0000;
        }
        
        input[type="text"], input[type="password"], input[type="number"] {
            width: 100%;
            padding: 12px;
            border: 1px solid rgba(255, 0, 0, 0.5);
            border-radius: 3px;
            background: rgba(5, 5, 5, 0.95);
            color: #ffffff;
            font-size: 1rem;
            letter-spacing: 1px;
            transition: all 0.2s ease;
        }
        
        input:focus { 
            outline: none;
            border-color: #ff0000;
            box-shadow: 0 0 10px rgba(255, 0, 0, 0.6);
            background: rgba(10, 0, 0, 0.95);
        }
        
        button {
            width: 100%;
            padding: 14px;
            background: #ff0000;
            border: 1px solid #ffffff;
            border-radius: 3px;
            color: #ffffff;
            font-size: 0.95rem;
            font-weight: bold;
            text-transform: uppercase;
            letter-spacing: 2px;
            cursor: pointer;
            transition: all 0.2s ease;
            box-shadow: 0 0 15px rgba(255, 0, 0, 0.5);
            margin-top: 10px;
        }
        
        button:hover { 
            background: #ffffff;
            color: #050508;
            box-shadow: 0 0 20px rgba(255, 255, 255, 0.7);
            border-color: #ff0000;
        }

        #status-msg {
            text-align: center;
            color: #ffffff;
            background: #ff0000;
            font-weight: bold;
            font-size: 0.9rem;
            letter-spacing: 2px;
            text-transform: uppercase;
            padding: 8px;
            margin-top: 12px;
            border-radius: 3px;
            box-shadow: 0 0 10px rgba(255,0,0,0.5);
            display: none;
        }
        
        @media (max-width: 768px) {
            .clock-title { font-size: 2rem; margin-bottom: 10px; }
            .time-display { font-size: 3.5rem; }
            .date-display { font-size: 1.75rem; }
            .screen-logo { max-width: 60px; top: 15px; left: 15px; }
            .telemetry-bar { flex-direction: column; gap: 4px; bottom: 8px; left: 15px; font-size: 0.75rem; }
        }
    </style>
    <link href="https://googleapis.com" rel="stylesheet">
</head>
<body>

    <!-- Top Half Structure -->
    <div class="top-section">
        <div class="clock-panel">
            <div class="clock-title">Source Time Date</div>
            <div class="time-display" id="time-1">--:--:--</div>
            <div class="date-display" id="date-1">--.--.----</div>
        </div>
        
        <div class="clock-panel">
            <div class="clock-title">UTC Time Date</div>
            <div class="time-display" id="time-2">--:--:--</div>
            <div class="date-display" id="date-2">--.--.----</div>
        </div>

        <!-- Telemetry Node Columns -->
        <div class="telemetry-bar">
            <div id="stat-atomic">// SIGNAL: ---</div>
            <div id="stat-wifi">// WIFI: ---</div>
            <div id="stat-eth">// ETH: ---</div>
        </div>
    </div>

    <!-- Bottom Half Structure -->
    <div class="bottom-section">
        <!-- Floating Corner Logo Image Asset -->
        <img src="/logo.jpg" class="screen-logo" alt="System Logo">

        <form id="configForm" onsubmit="submitConfigForm(event)">
            <div class="form-group">
                <label>WiFi Name (SSID)</label>
                <input type="text" name="ssid" id="field-ssid" autocomplete="off" required>
            </div>
            <div class="form-group">
                <label>WiFi Security Key</label>
                <input type="password" name="password" id="field-pass">
            </div>
            <div class="form-group">
                <label>UTC Offset (in Hours. Usually zero)</label>
                <input type="number" name="offset" id="field-offset" min="-12" max="12" value="0" required>
            </div>
            <button type="submit">Execute Configuration</button>
            <div id="status-msg">Configuration Saved!</div>
        </form>
    </div>

    <script>
        function fetchArduinoData() {
            fetch('/live-data')
                .then(response => response.json())
                .then(data => {
                    // Update main clocks
                    document.getElementById('time-1').innerText = data.t1;
                    document.getElementById('date-1').innerText = data.d1;
                    document.getElementById('time-2').innerText = data.t2;
                    document.getElementById('date-2').innerText = data.d2;
                    
                    // Update diagnostics bar
                    document.getElementById('stat-atomic').innerText = data.s_atom;
                    document.getElementById('stat-wifi').innerText = data.s_wifi;
                    document.getElementById('stat-eth').innerText = data.s_eth;
                    
                    // Prevent typewriter reset on active fields
                    if(document.activeElement.id !== 'field-ssid' && !document.getElementById('field-ssid').value) {
                        document.getElementById('field-ssid').value = data.saved_ssid;
                    }
                    if(document.activeElement.id !== 'field-pass' && !document.getElementById('field-pass').value) {
                        document.getElementById('field-pass').value = data.saved_pass;
                    }
                    if(document.activeElement.id !== 'field-offset' && !document.getElementById('field-offset').value) {
                        document.getElementById('field-offset').value = data.saved_offset;
                    }
                })
                .catch(err => console.error("Link offline: ", err));
        }

        // REPLACE THIS FUNCTION INSIDE YOUR CONFIGWEBPAGE.H
        function submitConfigForm(event) {
            event.preventDefault(); 
            
            const btn = event.target.querySelector('button');
            const msg = document.getElementById('status-msg');
            
            // 1. If the button is already locked out, drop this save click completely
            if (btn.disabled) return;
            
            // 2. Lock the save button instantly to stop fast double-clicking
            btn.disabled = true;
            btn.style.opacity = "0.5";
            btn.innerText = "PROCESSING...";

            const form = document.getElementById('configForm');
            const formData = new FormData(form);

            fetch('/save', { method: 'POST', body: formData })
            .then(response => {
                if (response.ok) {
                    msg.innerText = "Configuration Saved!";
                    msg.style.display = 'block';
                    setTimeout(() => { msg.style.display = 'none'; }, 5000);
                } else if (response.status === 429) {
                    // Catches the firewall rejection from the Arduino block
                    msg.innerText = "⚠️ ERROR: TOO MANY REQUESTS";
                    msg.style.display = 'block';
                }
            })
            .catch(err => console.error("Save failed: ", err))
            .finally(() => {
                // 3. THE LOCKOUT CLOCK: Re-enable the button precisely after 5000ms (5 seconds)
                setTimeout(() => {
                    btn.disabled = false;
                    btn.style.opacity = "1";
                    btn.innerText = "Execute Configuration";
                }, 2000);
            });
        }


        setInterval(fetchArduinoData, 250);
        window.onload = fetchArduinoData;
    </script>
</body>
</html>
)=====";

#endif /*WEBPAGE_H*/

