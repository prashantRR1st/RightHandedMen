import React, { Component, useState, useEffect, useRef } from 'react';
import {
    LineChart,
    ResponsiveContainer,
    Legend, Tooltip,
    Line,
    XAxis,
    YAxis,
    CartesianGrid
} from 'recharts';
import useInterval from "./useInterval";
import results from "../generated_input.json"

export default function WorkoutChart() {
    let [data, setData] = useState();

    useInterval(() => {
        const fetchData = async () => {
            try {
                const res = await fetch('http://127.0.0.1:8887/tracking_output.json', {mode:'cors'});
                const json = await res.json();
                let dataLen = json.length; 
                
                let results_len = results['buffer'].length;
                let xScale = 5;
                let startIndex = dataLen>(results_len*xScale)? dataLen-(results_len*xScale) : 0;
                let truncatedData = json.slice(startIndex, dataLen);
                let displayData = [];

                truncatedData.forEach((element, index) => {
                    
                    let profile_index = results_len > index? index : index%results_len;
                    displayData.push(
                        {
                            time: index,
                            live_force: element,
                            profile: results['buffer'][profile_index]
                        }
                    );
                })
                setData(displayData);
              }
              catch(err) {
                return;
              }
            
        };
        fetchData();
      }, 10);


    return(
        <div style={{backgroundColor: '#121212'}}>
            <ResponsiveContainer width="99%" aspect={3}>
                <LineChart data={data} margin={{ left: 80, right: 100, top: 30, bottom: 30 }}>
                    <CartesianGrid stroke="#404040" />
                        <XAxis dataKey="time (s)" interval={'preserveStartEnd'} />
                    <YAxis></YAxis>
                    <Legend />
                    <Tooltip />
                    <Line dataKey="live_force" stroke="#119db0" strokeWidth={2} activeDot={{ r: 8 }} />
                    <Line dataKey="profile" stroke="#f15b04"  strokeWidth={2} activeDot={{ r: 8 }} />
                </LineChart>
            </ResponsiveContainer>
            
        </div> 
    );
};