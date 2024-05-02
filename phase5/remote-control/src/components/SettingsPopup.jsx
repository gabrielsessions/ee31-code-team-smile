/* eslint-disable react/prop-types */

import { Dialog, Transition } from '@headlessui/react'
import { Fragment, useState } from 'react'
import { LOCALSTORAGE } from './WebSocketLog'

export default function SettingsPopup(props) {
  console.log(props.settings)
  let [isOpen, setIsOpen] = useState(false)

  function closeModal() {
    setIsOpen(false)
  }

  function openModal() {
    setIsOpen(true)
  }

  return (
    <>
      <div className="inline">
        <button
          type="button"
          onClick={openModal}
          className="inline-flex items-center justify-center h-12 px-2 text-sm font-semibold tracking-wide text-white transition duration-300 rounded focus-visible:outline-none whitespace-nowrap bg-blue-500 hover:bg-blue-600 focus:bg-blue-700 disabled:cursor-not-allowed disabled:border-blue-300 disabled:bg-blue-300 disabled:shadow-none"
        >
          Settings
        </button>
      </div>

      <Transition appear show={isOpen} as={Fragment}>
        <Dialog as="div" className="relative z-10" onClose={closeModal}>
          <Transition.Child
            as={Fragment}
            enter="ease-out duration-300"
            enterFrom="opacity-0"
            enterTo="opacity-100"
            leave="ease-in duration-200"
            leaveFrom="opacity-100"
            leaveTo="opacity-0"
          >
            <div className="fixed inset-0 bg-black/25" />
          </Transition.Child>

          <div className="fixed inset-0 overflow-y-auto">
            <div className="flex min-h-full items-center justify-center p-4 text-center">
              <Transition.Child
                as={Fragment}
                enter="ease-out duration-300"
                enterFrom="opacity-0 scale-95"
                enterTo="opacity-100 scale-100"
                leave="ease-in duration-200"
                leaveFrom="opacity-100 scale-100"
                leaveTo="opacity-0 scale-95"
              >
                <Dialog.Panel className="w-full max-w-md transform overflow-hidden rounded-2xl bg-white p-6 text-left align-middle shadow-xl transition-all">
                  <Dialog.Title
                    as="h3"
                    className="text-lg font-medium leading-6 text-gray-900"
                  >
                    Control Panel Settings
                  </Dialog.Title>
                  <div className="mt-2">
                    <p className="text-sm text-gray-500">
                      Please enter your button configurations here:
                    </p>
                  </div>
                  <div>
                    {
                      props.settings ?
                        <div className='space-y-1'>
                          {Object.keys(props.settings).map((field) => {
                            return (
                              <>

                                <label>{field}: </label>
                                <input

                                  type="text"

                                  placeholder="Value..."
                                  value={props.settings[field]}
                                  className="peer inline h-10 w-48 rounded border border-slate-200 px-4 text-sm text-slate-500 placeholder-transparent outline-none transition-all autofill:bg-white invalid:border-pink-500 invalid:text-pink-500 focus:border-blue-500 focus:outline-none invalid:focus:border-pink-500 focus-visible:outline-none disabled:cursor-not-allowed disabled:bg-slate-50 disabled:text-slate-400 my-2"
                                  onChange={(e) => {
                                    props.setSettings((prev) => {
                                      const copy = {...prev};
                                      copy[field] = e.target.value;
                                      return copy;
                                    }) 
                                  }}
                                />
                                <br />
                              </>
                            );


                          })}
                        </div>
                        :
                        <p>Problem fetching settings...</p>
                    }
                  </div>

                  <div className="mt-4">
                    <button
                      type="button"
                      className="inline-flex justify-center rounded-md border border-transparent bg-blue-100 px-4 py-2 text-sm font-medium text-blue-900 hover:bg-blue-200 focus:outline-none focus-visible:ring-2 focus-visible:ring-blue-500 focus-visible:ring-offset-2"
                      onClick={() => {
                        localStorage.setItem(LOCALSTORAGE, JSON.stringify(props.settings))
                        closeModal();
                      }}
                    >
                      Save
                    </button>

                    <button
                      type="button"
                      className="inline-flex justify-center rounded-md border border-transparent bg-red-100 px-4 py-2 text-sm font-medium text-red-900 hover:bg-red-200 focus:outline-none focus-visible:ring-2 focus-visible:ring-red-500 focus-visible:ring-offset-2 mx-8"
                      onClick={props.resetSettings}
                    >
                      Reset All Settings
                    </button>
                  </div>
                </Dialog.Panel>
              </Transition.Child>
            </div>
          </div>
        </Dialog>
      </Transition>
    </>
  )
}
