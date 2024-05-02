/* eslint-disable react/prop-types */

export default function RobotController(props) {

  const sendCommand = (command) => {
    console.log(command);
    props.sendMessage(`${props.settings[command]}`)
    // Implement the function to communicate with the robot's movement system here
  };

  return (
    <div className="p-5">
      <h1 className="text-lg font-bold mb-4 inline">Robot Control Panel</h1>
      <button
          className="bg-red-500 hover:bg-red-700 text-white font-bold py-2 px-4  rounded my-4 ml-4"
          onClick={() => sendCommand("stop")}
        >
          stop
        </button>
      <div className="flex justify-center space-x-4 mb-2">
        <button
          className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"
          onClick={() => sendCommand("f_fwd")}
        >
          F_Fwd
        </button>

      </div>

      <div className="flex justify-center space-x-4 mb-2">

        <button
          className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"
          onClick={() => sendCommand("s_fwd")}
        >
           S_Fwd
        </button>
      </div>

      <div className="flex justify-between mb-2">
        <div className="md:space-x-2 space-y-2">
          <button
            className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"
            onClick={() => sendCommand("f_lft")}
          >
             F_Lft
          </button>
          <button
            className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"
            onClick={() => sendCommand("s_lft")}
          >
             S_Lft
          </button>
        </div>
        <div className="md:space-x-2 space-y-2">
          <button
            className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"
            onClick={() => sendCommand("s_rht")}
          >
             S_Rht
          </button>
          <button
            className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"
            onClick={() => sendCommand("f_rht")}
          >
             F_Rht
          </button>
        </div>
      </div>

      <div className="flex justify-center space-x-4 mb-2">
        <button
          className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"
          onClick={() => sendCommand("s_bwd")}
        >
           S_Bwd
        </button>

      </div>

      <div className="flex justify-center space-x-4 mb-2">

        <button
          className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"
          onClick={() => sendCommand("f_bwd")}
        >
          F_Bwd
        </button>
      </div>


      <div className="space-x-2 mt-8">
        
        <button
          className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4  rounded"
          onClick={() => sendCommand("blue")}
        >
          blue
        </button>
        <button
          className="bg-yellow-500 hover:bg-yellow-700 text-white font-bold py-2 px-4  rounded"
          onClick={() => sendCommand("yellow")}
        >
          yellow
        </button>
        <button
          className="bg-red-500 hover:bg-red-700 text-white font-bold py-2 px-4  rounded"
          onClick={() => sendCommand("red")}
        >
          red
        </button>
      </div>

      <div className="space-x-2 my-2">
        
        <button
          className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4  rounded"
          onClick={() => sendCommand("collision")}
        >
          collision
        </button>
        <button
          className="bg-gray-500 hover:bg-gray-700 text-white font-bold py-2 px-4  rounded"
          onClick={() => sendCommand("night")}
        >
          night
        </button>
        <button
          className="bg-white hover:bg-gray-100 text-black border  font-bold py-2 px-4  rounded"
          onClick={() => sendCommand("day")}
        >
          day
        </button>
        <button
          className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4  rounded"
          onClick={() => sendCommand("buzzer")}
        >
          buzzer
        </button>
      </div>

      <div className="space-x-2 my-2">
        
        <button
          className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4  rounded"
          onClick={() => sendCommand("custom1")}
        >
          custom1
        </button>
        <button
          className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4  rounded"
          onClick={() => sendCommand("custom2")}
        >
          custom2
        </button>
        <button
          className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4  rounded"
          onClick={() => sendCommand("custom3")}
        >
          custom3
        </button>
        <button
          className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4  rounded"
          onClick={() => sendCommand("custom4")}
        >
          custom4
        </button>
      </div>


    </div>
  );
}
