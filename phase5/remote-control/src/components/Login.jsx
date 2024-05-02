/* eslint-disable react/prop-types */

export default function Login(props) {

  function teamSelected(team) {
    props.setClientID(team);
    props.onSubmit(team);
  }

  return (
    <div className="bg-white shadow-lg rounded-lg p-8 m-4 w-full max-w-lg block">
      <h2 className="font-bold text-2xl mb-4">EE31 Remote Control Client</h2>
      <img className="h-40 md:h-48 lg:h-64 xl:h-72 mx-auto my-6" src="/PCB_Image.jpg"></img>
      <p>Select your EE 31 Team:</p>

      <div className="space-x-6 my-4">
        <button
          className="inline-flex items-center justify-center h-12 gap-2 px-6 text-sm font-semibold tracking-wide text-white transition duration-300 rounded focus-visible:outline-none whitespace-nowrap bg-blue-500 hover:bg-blue-600 focus:bg-blue-700 disabled:cursor-not-allowed disabled:border-blue-300 disabled:bg-blue-300 disabled:shadow-none mb-4"
          onClick={() => teamSelected("4A9EDB0160D5")}
        >
          <span>{"Team :)"}</span>
        </button>
        <button
          className="inline-flex items-center justify-center h-12 gap-2 px-6 text-sm font-semibold tracking-wide text-white transition duration-300 rounded focus-visible:outline-none whitespace-nowrap bg-blue-500 hover:bg-blue-600 focus:bg-blue-700 disabled:cursor-not-allowed disabled:border-blue-300 disabled:bg-blue-300 disabled:shadow-none mb-4"
          onClick={() => teamSelected("DCF2BCAB6F0B")}
        >
          <span>{"Team Infra Red"}</span>
        </button>
      </div>




    </div>
  )
}