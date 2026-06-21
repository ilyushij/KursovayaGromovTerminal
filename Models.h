#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace msclr::interop;

public ref class Operator
{
private:
	int id;
	String^ name;
	double commission;
	String^ phoneMask;

public:
	Operator(int id, String^ name, double commission, String^ phoneMask)
	{
		this->id = id;
		this->name = name;
		this->commission = commission;
		this->phoneMask = phoneMask;
	}

	property int Id { int get() { return id; } void set(int value) { id = value; } }
	property String^ Name { String^ get() { return name; } void set(String^ value) { name = value; } }
	property double Commission { double get() { return commission; } void set(double value) { commission = value; } }
	property String^ PhoneMask { String^ get() { return phoneMask; } void set(String^ value) { phoneMask = value; } }

	bool ValidatePhone(String^ phone)
	{
		if (String::IsNullOrWhiteSpace(phone)) 
		{
			return false;
		}

		String^ cleanPhone = phone->Replace(" ", "")->Replace("-", "")->Replace("(", "")->Replace(")", "");

		if (cleanPhone->Length == 11 && cleanPhone->StartsWith(phoneMask))
		{
			for (int i = 0; i < cleanPhone->Length; ++i)
			{
				if (!Char::IsDigit(cleanPhone[i]))
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}

	String^ ToString() override
	{
		return String::Format("{0}|{1}|{2}|{3}", id, name, commission, phoneMask);
	}

	static Operator^ Parse(String^ line)
	{
		array<String^>^ parts = line->Split('|');
		
		if (parts->Length != 4)
		{
			throw gcnew Exception("Неверный формат строки оплаты");
	
		}
		int id = Convert::ToInt32(parts[0]);
		String^ name = parts[1];
		double comm = Convert::ToDouble(parts[2]);
		String^ mask = parts[3];

		return gcnew Operator(id, name, comm, mask);
	}
};

public ref class Payment {
private:
	int receiptNumber;
	int operatorId;
	String^ phoneNumber;
	double amount;
	DateTime dateTime;

public:

	Payment(int receiptNumber, int operatorId, String^ phoneNumber, double amount, DateTime dateTime) {
		this->receiptNumber = receiptNumber;
		this->operatorId = operatorId;
		this->phoneNumber = phoneNumber;
		this->amount = amount;
		this->dateTime = dateTime;
	}


	property int ReceiptNumber { int get() { return receiptNumber; } }
	property int OperatorId { int get() { return operatorId; } }
	property String^ PhoneNumber { String^ get() { return phoneNumber; } }
	property double Amount { double get() { return amount; } }
	property DateTime DateTime { System::DateTime get() { return dateTime; } }


	String^ ToString() override {

		String^ dateStr = dateTime.ToString("dd-MM-yyyy HH:mm:ss");
		return String::Format("{0}|{1}|{2}|{3}|{4}", receiptNumber, operatorId, phoneNumber, amount, dateStr);
	}


	static Payment^ Parse(String^ line) {
		array<String^>^ parts = line->Split('|');
		if (parts->Length != 5) throw gcnew Exception("Неверный формат строки платежа");

		int recNum = Convert::ToInt32(parts[0]);
		int opId = Convert::ToInt32(parts[1]);
		String^ phone = parts[2];
		double amt = Convert::ToDouble(parts[3]);
		DateTime dt = DateTime::ParseExact(parts[4], "dd-MM-yyyy HH:mm:ss", nullptr);

		return gcnew Payment(recNum, opId, phone, amt, dt);
	}

	generic <typename T>

	public ref class DataRepository {
	private:
		array<T>^ data;
		int size;
		int capacity;


		void Resize(int newCapacity) {
			array<T>^ newData = gcnew array<T>(newCapacity);
			if (data != nullptr && size > 0) {
				Array::Copy(data, newData, size);
			}
			data = newData;
			capacity = newCapacity;
		}

	public:

		DataRepository()
		{
			capacity = 4;
			size = 0;
			data = gcnew array<T>(capacity);
		}

		void Add(T item) {
			if (size == capacity) {
				Resize(capacity * 2);
			}
			data[size] = item;
			size++;
		}


		T Get(int index) {
			if (index < 0 || index >= size) {
				throw gcnew IndexOutOfRangeException("Индекс находится за пределами коллекции");
			}
			return data[index];
		}


		void RemoveAt(int index) {
			if (index < 0 || index >= size) {
				throw gcnew IndexOutOfRangeException("Индекс находится за пределами коллекции");
			}

			for (int i = index; i < size - 1; i++) {
				data[i] = data[i + 1];
			}
			size--;
		}


		void Clear() {
			size = 0;
		}


		property int Count {
			int get() { return size; }
		}


		property array<T>^ Items {
			array<T>^ get() {
				array<T>^ result = gcnew array<T>(size);
				Array::Copy(data, result, size);
				return result;
			}
		}
	};
};






