/* eslint-disable react/prop-types */

import { useEffect, useRef, useState } from "react"


export default function WebSocketLog(props) {
  const messagesRef = useRef(null);
  const [messageInput, setMessageInput] = useState("");

  const handleChange = event => {
    setMessageInput(event.target.value);
  }

  useEffect(() => {
    messagesRef.current.scrollTop = messagesRef.current.scrollHeight;
  }, [props])

  return (
    <>
      <div className="bg-white shadow-lg rounded-lg p-8 m-4 w-full max-w-5xl block">
        <h2 className="font-bold text-2xl mb-4">Currently Connected as <span className="italic text-blue-500">WebClient_{props.clientID}</span></h2>
        <div className="flex justify-center p-4">
          <div className="w-full max-w-2xl bg-white shadow rounded">
            <div className="p-4">
              <h2 className="font-semibold text-lg">Log of Server Messages</h2>
            </div>
            <div className="overflow-auto h-64 md:h-72 lg:min-h-80" ref={messagesRef} style={{ scrollBehavior: "smooth" }}>
              {props.log.map((item, index) => (
                <div key={index} className="p-2 border-b border-gray-200">
                  {item}
                </div>
              ))}
            </div>
          </div>
        </div>
        <div className="mx-4">
          <p className="md:inline">Send Message: </p>
          <form className="md:inline" onSubmit={(e) => {
            e.preventDefault();
            props.sendMessage(messageInput);
            setMessageInput("");
          }}>
            <input
              id="id-01"
              type="text"
              name="id-01"
              placeholder="Enter a message here..."
              value={messageInput}
              className="peer md:inline h-10 max-w-96 w-full rounded border border-slate-200 px-4 text-sm text-slate-500 placeholder-transparent outline-none transition-all autofill:bg-white invalid:border-pink-500 invalid:text-pink-500 focus:border-blue-500 focus:outline-none invalid:focus:border-pink-500 focus-visible:outline-none disabled:cursor-not-allowed disabled:bg-slate-50 disabled:text-slate-400 my-3 md:mx-4"
              onChange={handleChange}
            />

            <button className="md:inline-flex items-center justify-center h-12 gap-2 px-6 text-sm font-semibold tracking-wide text-white transition duration-300 rounded focus-visible:outline-none whitespace-nowrap bg-blue-500 hover:bg-blue-600 focus:bg-blue-700 disabled:cursor-not-allowed disabled:border-blue-300 disabled:bg-blue-300 disabled:shadow-none mb-4" type="submit">
              <span>Send</span>
            </button>
          </form>

        </div>

        <button className="md:inline-flex items-center justify-center h-12 gap-2 px-6 text-sm font-semibold tracking-wide text-white transition duration-300 rounded focus-visible:outline-none whitespace-nowrap bg-red-500 hover:bg-red-600 focus:bg-red-700 disabled:cursor-not-allowed disabled:border-red-300 disabled:bg-red-300 disabled:shadow-none my-2 mx-4" onClick={props.disconnect}>
          <span>Disconnect From Server</span>
        </button>

      </div>
    </>
  )
}