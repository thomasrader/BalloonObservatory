library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity full2HalfDueplex is
    Port ( Control : in  	STD_LOGIC;
           Tx      : in  	STD_LOGIC_VECTOR (3 downto 0);
           Rx      : out  	STD_LOGIC_VECTOR (3 downto 0);
           Data    : inout STD_LOGIC_VECTOR (3 downto 0));
end full2HalfDueplex;

architecture full2HalfDueplex_arch of full2HalfDueplex  is

signal data_int_tx : STD_LOGIC;
signal data_int_rx : STD_LOGIC;

begin
	--Wire data to internal lines
	data_int_rx <= Data;
	Data <= data_int_tx;
	
	--Tristate Buffers
   data_int_tx <= Tx when (Control = '1') else 'Z';
	Rx <= data_int_rx when (Control = '0') else 'Z';
end architecture;