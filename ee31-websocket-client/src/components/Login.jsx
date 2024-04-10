/* eslint-disable react/prop-types */

export default function Login(props) {
  const handleChange = evt => {
    const value = evt.target.value
    props.setClientID(value);
  }

  return (
    <div className="bg-white shadow-lg rounded-lg p-8 m-4 w-full max-w-lg block">
      <h2 className="font-bold text-2xl mb-4">Welcome to the EE31 Socket Client!</h2>
      <img className="h-40 md:h-48 lg:h-64 xl:h-72 mx-auto my-6" src="./login_image.jpg"></img>
      <p>Please log into the client using your assigned Server ID.</p>
      <form onSubmit={(e) => { 
        e.preventDefault(); 
        props.onSubmit(props.clientID);
      }}>
        <input
          id="id-01"
          type="text"
          name="id-01"
          placeholder="Server ID"
          value={props.clientID}
          className="peer relative h-10 w-full rounded border border-slate-200 px-4 text-sm text-slate-500 placeholder-transparent outline-none transition-all autofill:bg-white invalid:border-pink-500 invalid:text-pink-500 focus:border-blue-500 focus:outline-none invalid:focus:border-pink-500 focus-visible:outline-none disabled:cursor-not-allowed disabled:bg-slate-50 disabled:text-slate-400 my-3"
          onChange={handleChange}
        />

        <button className="inline-flex items-center justify-center h-12 gap-2 px-6 text-sm font-semibold tracking-wide text-white transition duration-300 rounded focus-visible:outline-none whitespace-nowrap bg-blue-500 hover:bg-blue-600 focus:bg-blue-700 disabled:cursor-not-allowed disabled:border-blue-300 disabled:bg-blue-300 disabled:shadow-none mb-4" type="submit">
          <span>Connect to Server</span>
        </button>
      </form>

      <p>Unsure of what your server ID is?</p> <p>You can find it in the <a className="text-blue-500 hover:underline hover:text-blue-700 active:text-blue-800" rel="roreferrer" target="_blank" href="https://docs.google.com/spreadsheets/d/1udJJf3jWEN1POSr1noUS1HA-zv0qLAz7snsEI_BNKLk/edit#gid=679216118">EE 31 Progress Spreadsheet</a></p>
    </div>
  )
}