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
            const res = await fetch('http://127.0.0.1:8887/tracking_output.json', {mode:'cors'});
            const json = await res.json();
            let displayData = [];

            json.forEach((element, index) => {
                let results_len = results['buffer'].length;
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
        };
        fetchData();
      }, 10);


    return(
        <div>
            <ResponsiveContainer width="100%" aspect={3}>
                <LineChart data={data} margin={{ left: 100, right: 100, top: 30, bottom: 30 }}>
                    <CartesianGrid />
                        <XAxis dataKey="time (s)" interval={'preserveStartEnd'} />
                    <YAxis></YAxis>
                    <Legend />
                    <Tooltip />
                    <Line dataKey="live_force" stroke="black" activeDot={{ r: 8 }} />
                    <Line dataKey="profile" stroke="red" activeDot={{ r: 8 }} />
                </LineChart>
            </ResponsiveContainer>
            
        </div> 
    );
};