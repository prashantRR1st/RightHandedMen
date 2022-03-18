import { Component, useState, useEffect } from "react";
import {
    LineChart,
    ResponsiveContainer,
    Legend, Tooltip,
    Line,
    XAxis,
    YAxis,
    CartesianGrid
} from 'recharts';

export default function SummaryChart(props) {    
    
    return(
        <div>
            <ResponsiveContainer width="99%" aspect={3}>
                <LineChart data={props.data} >
                    <CartesianGrid stroke="#404040"/>
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