// Package Imports
import { WebSocketServer } from 'ws';
import 'dotenv/config';

// Initialize the Web Socket Server
// PORT must be specified in a .env file
const wss = new WebSocketServer({ port: process.env.PORT });


// Storing the clients that have an active connection to the server
const clients = {};
const validClients = [
    "4A9EDB0160D5",
    "F79721857DC5",
    "56FC703ACE1A",
    "DCF2BCAB6F0B",
    "828BD9E1B7C7",
    "89C87865077A",
    "8050D1451904",
    "F392FC86D8D7",

    "WebClient_4A9EDB0160D5",
    "WebClient_F79721857DC5",
    "WebClient_56FC703ACE1A",
    "WebClient_DCF2BCAB6F0B",
    "WebClient_828BD9E1B7C7",
    "WebClient_89C87865077A",
    "WebClient_8050D1451904",
    "WebClient_F392FC86D8D7",
]

// Websocket logic
wss.on('connection', (ws) => {
    ws.on('error', console.error);
    ws.send("SERVER Send Client ID");
    let clientID = null;

    ws.on('message', (data) => {

        if (clientID === null) {
            if (!data || !validClients.includes(data.toString())) {
                ws.send("SERVER Invalid Client ID");
                ws.close();
                return;
            }
            clientID = data.toString();
            if (clients[clientID]) {
                clientID = clientID + "_" + Math.round(Math.random()*10000);
            }
            
            clients[clientID] = ws;
            console.log(clients);
            return;
        }
        
        if (!clients[clientID]) {
            clients[clientID] = ws;
        }

        console.log(clientID + "," + data.toString());
        
        Object.keys(clients).forEach(client => {
            try {
                if (clients[client]) {
                    clients[client].send(clientID + "," + data.toString());
                }
            }
            catch (error) {
                console.log(client);
                console.log(error);
            }
            
        })

    });
    ws.on('close', () => { clients[clientID] = undefined })
});




console.log("Running server on port " + process.env.PORT || 3000)