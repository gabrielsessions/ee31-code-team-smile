import { useState } from 'react'
import Login from './components/Login';
import WebSocketLog from './components/WebSocketLog';


function App() {

  const [WS, setWS] = useState(null);
  const [clientID, setClientID] = useState("");
  const [messages, setMessages] = useState([]);
  const [popup, setPopup] = useState({
    visible: false,
    message: "Alert!"
  })

  const showAlert = message => {
    setPopup({
      visible: true,
      message: message
    })
    setTimeout(() => {
      setPopup({
        visible: false,
        message: ""
      })
    }, 3000);
  }

  const initialConnect = (clientID) => {
    if (!clientID) {
      return;
    }
    const socket = new WebSocket('ws://34.173.203.110');
    socket.addEventListener("open", () => {
      console.log("Client Open!");
      socket.send("WebClient_" + clientID);
      setWS(socket);
      socket.send("Hello!");
    });
    socket.addEventListener("close", () => {
      console.log("Socket Closed!");
      setWS(null);
      showAlert("Socket connection closed. If this was not intentional, please reconnect!")
    })
    socket.addEventListener("message", (message) => {
      console.log(message);
      setMessages((prev) => [...prev, message.data]);
    })

  }


  // Fetch localstorage stuff
  

  return (
    <>

      <div className="flex justify-center items-center min-h-screen bg-gray-200">
        <div className="absolute top-2 right-2">
          {
            popup.visible ?
              <div className="w-full px-4 py-3 text-sm border rounded border-blue-100 bg-blue-50 text-blue-500" role="alert">
                <p>{popup.message}</p>
              </div>
              :
              <></>
          }
        </div>

        {
          WS ?
            <>
              <WebSocketLog clientID={clientID} log={messages} sendMessage={(message) => WS.send(message)} disconnect={() => {
                WS.close();
              }} />
            </> :
            <Login onSubmit={initialConnect} clientID={clientID} setClientID={setClientID} />


        }

      </div>


    </>

  )
}

export default App;
